#include "fanartthread.h"

#include <QVariant>

#include <QFile>
#include <QImage>
#include <QDebug>

//#include "../frameworks/scraping/providers/htbackdrops.h"
#include "../frameworks/scraping/providers/plexusartist.h"
#include <plxFramework/media/mediaitem.h>
#include <plxFramework/application/Settings.h>
#include "../utils/Image.h"

/*******************************************************************************
  Fanart Search Thread
*******************************************************************************/
FanartThread::FanartThread(QObject *parent) : QThread(parent)
{
  setObjectName("fanartthread");
  setTerminationEnabled(true);

  //! init
  m_currentRequestId  = 0;
  m_bFullDBSearch     = true;
  m_bExit             = false;

  m_musicDatabase = CMusicDatabase::instance();
}

FanartThread::~FanartThread()
{
}

void FanartThread::setRequest(const QString& artist)
{
  if (artist.isEmpty()) return;

  ProviderParam param;

  param[ARTIST]         = artist;
  param[FANART]         = CMediaItem::buildImageName(artist, "");
  param[FANART_THUMB]   = CMediaItem::buildImageName(artist, "", true);

  const QString filePath = CSettings::folder(CSettings::FOLDER_FANARTS) + param[FANART_THUMB].toString();

  if(!QFile::exists(filePath))
  {
    m_workList.append(param);
  }

  m_bFullDBSearch = false;
}

void FanartThread::run()
{
  qDebug() << "INFO: FanartManager -> Running";
  if(m_bFullDBSearch)
  {
    qDebug() << "INFO: FanartManager -> Full database check";
    checkDatabase();
  }

  m_iMax = m_workList.size();

  processSearch();

  //! Start thread event loop --> makes signals and slots work
  exec();
  qDebug() <<"FanartManager -> Exited";

  m_bFullDBSearch      = true;
  m_workList.clear();
}

void FanartThread::searchFinished()
{
  qDebug() << "FanartManager -> Finished";
  emit progress ( "Atualizando imagens de fundo", "Finalizando", m_iMax, m_iMax);
  emit finished();
  exit();
}

void FanartThread::processSearch()
{
  //! deletes previous providers
  qDeleteAll(m_providers);
  m_providers.clear();

  qDebug() << "FanartManager -> Processing Search";

  if(m_workList.isEmpty() || m_bExit)
  {
    searchFinished();
    return;
  }


  m_currentWork = m_workList.takeFirst();
  //  emit progress ( ( (m_iMax - m_workList.size()) * 100)/m_iMax );
  emit progress ( "Atualizando imagens de fundo", m_currentWork[ARTIST].toString(), (m_iMax - m_workList.size()), m_iMax);

  //! set providers
  m_providers.append( new CPlexusArtist() );
//  m_providers.append( new CHTBackdrops() );

  foreach (iProvider* provider, m_providers)
  {
    connect(provider, SIGNAL(notFound(int)), this, SLOT(notFound(int)));
    connect(provider, SIGNAL(finished(int)), this, SLOT(startProvider()));
    connect(provider, SIGNAL(dataReceived(int,QByteArray)), this, SLOT(saveFanart(int,QByteArray)));
  }

  //! start fanart provider
  m_currentProviderId = 0;
  startProvider();
}

void FanartThread::startProvider()
{
  if( m_currentProviderId < m_providers.size() )
  {
    iProvider* provider = m_providers.at(m_currentProviderId);

    provider->setParams(m_currentWork);
    provider->work(++m_currentRequestId);

    m_currentProviderId++;
  }
  else
  {
    //! process next request
    processSearch();
  }
}

void FanartThread::checkDatabase()
{
  qDebug() << "INFO: FanartThread -> Checking Database";

  if (!m_musicDatabase->open())
  {
    qDebug() << "ERROR: FanartThread -> Couldn't open database";
    return;
  }

  //! artist database loop
  int count = 0;
  int timeStamp(QDateTime::currentDateTime().toTime_t() - 259200/*3 days in miliseconds*/);

  QSqlQuery queryCount("SELECT count(idFanart) FROM fanartview WHERE ((iTimeStamp is NULL) or (iTimeStamp < "+QString::number(timeStamp)+")) ORDER BY strArtist",*m_musicDatabase->db());
  queryCount.first();
  int total = queryCount.value(0).toInt();

  QSqlQuery query("SELECT idFanart, strArtist, strFanart, strThumb FROM fanartview WHERE ((iTimeStamp is NULL) or (iTimeStamp < "+QString::number(timeStamp)+")) ORDER BY strArtist",*m_musicDatabase->db());
  while (query.next())
  {
    ProviderParam param;

    param[ID]  = query.value(0); // idFanart
    param[ARTIST]  = query.value(1); // artist
    param[FANART]  = query.value(2); // fanart
    param[FANART_THUMB] = query.value(3); // Fanart Thumb

    QString filePath = CSettings::folder(CSettings::FOLDER_FANARTS) + param[FANART_THUMB].toString();

    if(!QFile::exists(filePath))
    {
      qDebug() << "INFO: FanartThread -> Fanart not found for: " << param[ARTIST];
      m_workList.append(param);
      qDebug() << "INFO: FanartThread -> Selecting to search ";
    }
    count++;
    emit progress ( "Verificando imagens de fundo", query.value(1).toString(), count, total );
  } // fin while album query
//  m_musicDatabase->close();
}

void FanartThread::notFound(int id)
{
  //Update DataBase
  qDebug() << "DEBUG: FanartManager -> Updating not found fanart timestamp";
  int timeStamp(QDateTime::currentDateTime().toTime_t());
  QSqlQuery query("UPDATE fanart SET iTimeStamp = "+QString::number(timeStamp)+" WHERE idFanart = '"+m_currentWork[ID].toString()+"'",*m_musicDatabase->db());
  query.exec();
}

void FanartThread::saveFanart(int request_id, QByteArray bytes)
{
  qDebug() << "INFO: FanartManager -> Saving fanart";

  if(m_currentRequestId != request_id)
  {
    qDebug() << "ERROR: FanartManager -> Bad Request ID";
    startProvider();
    return;
  }

  //! filepath for fanart file
  const QString strThumbFile  = CSettings::folder(CSettings::FOLDER_FANARTS) + m_currentWork[FANART_THUMB].toString();
  const QString strFanartFile = CSettings::folder(CSettings::FOLDER_FANARTS) + m_currentWork[FANART].toString();
  const QString strProvider = "Plexus"; //CORRIGIR. Precisa ser dinâmico

  QFile file(strThumbFile);

  if(file.exists())
  {
    qDebug() << "INFO: FanartManager -> File exists, starting search for next fanart";
  }
  else
  {
    Image::save(strFanartFile, bytes, QSize(1280,720));
    Image::save(strThumbFile, bytes, QSize(960,540));
    //Update DataBase
    qDebug() << "DEBUG: FanartManager -> Updating saved fanart timestamp";
    int timeStamp(QDateTime::currentDateTime().toTime_t());
    QSqlQuery query("UPDATE fanart SET iTimeStamp = "+QString::number(timeStamp)+", strProvider = '"+strProvider+"' WHERE idFanart = '"+m_currentWork[ID].toString()+"'",*m_musicDatabase->db());
    query.exec();
  }

  //! process next request
  processSearch();
}
