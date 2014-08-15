#include "coverthread.h"

#include <QVariant>

#include <QFile>
#include <QImage>
#include <QDebug>

//#include "../frameworks/scraping/providers/htbackdrops.h"
#include "../frameworks/scraping/providers/plexusalbum.h"
#include <plxFramework/media/mediaitem.h>
#include <plxFramework/application/Settings.h>
#include "../utils/Image.h"

/*******************************************************************************
  Cover Search Thread
*******************************************************************************/
CoverThread::CoverThread(QObject *parent) : QThread(parent)
{
  setObjectName("coverthread");
  setTerminationEnabled(true);

  //! init
  m_currentRequestId  = 0;
  m_bFullDBSearch     = true;
  m_bExit             = false;

  m_musicDatabase = CMusicDatabase::instance();
}

CoverThread::~CoverThread()
{
}

void CoverThread::setRequest(const QString& artist, const QString& title)
{
  if (artist.isEmpty() || title.isEmpty()) return;

  ProviderParam param;

  param[ARTIST]        = artist;
  param[TITLE]         = title;
  param[COVER]         = CMediaItem::buildImageName(artist, title);
  param[COVER_THUMB]   = CMediaItem::buildImageName(artist, title, true);

  const QString filePath = CSettings::folder(CSettings::FOLDER_ALBUMS) + param[COVER_THUMB].toString();

  if(!QFile::exists(filePath))
  {
    m_workList.append(param);
  }

  m_bFullDBSearch = false;
}

void CoverThread::run()
{
  qDebug() << "INFO: CoverManager -> Running";
  if(m_bFullDBSearch)
  {
    qDebug() << "INFO: CoverManager -> Full database check";
    checkDatabase();
  }

  m_iMax = m_workList.size();

  processSearch();

  //! Start thread event loop --> makes signals and slots work
  exec();
  qDebug() <<"CoverManager -> Exited";

  m_bFullDBSearch      = true;
  m_workList.clear();
}

void CoverThread::searchFinished()
{
  qDebug() << "CoverManager -> Finished";
  emit finished();
  exit();
}

void CoverThread::processSearch()
{
  //! deletes previous providers
  qDeleteAll(m_providers);
  m_providers.clear();

  qDebug() << "CoverManager -> Processing Search";

  if(m_workList.isEmpty() || m_bExit)
  {
    qDebug() << "CoverManager -> Vazio";
    searchFinished();
    return;
  }

  emit progress ( ( (m_iMax - m_workList.size()) * 100)/m_iMax );

  m_currentWork = m_workList.takeFirst();

  //! set providers
  m_providers.append( new CPlexusAlbum() );

  foreach (iProvider* provider, m_providers)
  {
    connect(provider, SIGNAL(notFound(int)), this, SLOT(notFound(int)));
    connect(provider, SIGNAL(finished(int)), this, SLOT(startProvider()));
    connect(provider, SIGNAL(dataReceived(int,QByteArray)), this, SLOT(saveCover(int,QByteArray)));
  }

  //! start cover provider
  m_currentProviderId = 0;
  startProvider();
}

void CoverThread::startProvider()
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

void CoverThread::checkDatabase()
{
  qDebug() << "INFO: CoverThread -> Checking Database";

  if (!m_musicDatabase->open())
  {
    qDebug() << "ERROR: CoverThread -> Couldn't open database";
    return;
  }

  //! album database loop
  int count = 0;
  int timeStamp(QDateTime::currentDateTime().toTime_t() - 259200/*3 days in miliseconds*/);

  QSqlQuery query("SELECT idCover, strArtist, strAlbum, strCover, strThumb FROM coverview WHERE ((iTimeStamp is NULL) or (iTimeStamp < "+QString::number(timeStamp)+")) ORDER BY strArtist, strAlbum",*m_musicDatabase->db());
  while (query.next())
  {
    ProviderParam param;

    param[ID]  = query.value(0); // artist
    param[ARTIST]  = query.value(1); // artist
    param[TITLE]  = query.value(2); // artist
    param[COVER]  = CMediaItem::buildImageName(param[ARTIST].toString(), param[TITLE].toString()); // cover
    param[COVER_THUMB] = CMediaItem::buildImageName(param[ARTIST].toString(), param[TITLE].toString(), true); // Cover Thumb

    QString filePath = CSettings::folder(CSettings::FOLDER_ALBUMS) + param[COVER_THUMB].toString();

//    if(!QFile::exists(filePath)) -- Não está ignorando mesmo que a capa exista.
    {
      qDebug() << "INFO: CoverThread -> Cover not found for: " << param[ARTIST] << " - " << param[TITLE];
      m_workList.append(param);
      qDebug() << "INFO: CoverThread -> Selecting to search ";
    }
    count++;
  } // fin while album query
//  m_musicDatabase->close();
}

void CoverThread::notFound(int id)
{
  //Update DataBase
  qDebug() << "DEBUG: CoverManager -> Updating not found cover timestamp";
  int timeStamp(QDateTime::currentDateTime().toTime_t());
  QSqlQuery query("UPDATE cover SET iTimeStamp = "+QString::number(timeStamp)+" WHERE idCover = '"+m_currentWork[ID].toString()+"'",*m_musicDatabase->db());
  query.exec();
}

void CoverThread::saveCover(int request_id, QByteArray bytes)
{
  qDebug() << "INFO: CoverManager -> Saving cover";

  if(m_currentRequestId != request_id)
  {
    qDebug() << "ERROR: CoverManager -> Bad Request ID";
    startProvider();
    return;
  }

  //! filepath for cover file
  const QString strCoverFile = CSettings::folder(CSettings::FOLDER_ALBUMS) + m_currentWork[COVER].toString();
  const QString strThumbFile = CSettings::folder(CSettings::FOLDER_ALBUMS) + m_currentWork[COVER_THUMB].toString();
  const QString strProvider = "Plexus"; //CORRIGIR. Precisa ser dinâmico

  QFile file(strThumbFile);

//  if(file.exists()) -- substitui a capa mesmo assim.
//  {
//    qDebug() << "INFO: CoverManager -> File exists, starting search for next cover";
//  }
//  else
  {
    Image::save(strCoverFile, bytes, QSize(300,300));
    Image::save(strThumbFile, bytes, QSize(200,200));
    //Update DataBase
    qDebug() << "DEBUG: CoverManager -> Updating saved cover timestamp";
    int timeStamp(QDateTime::currentDateTime().toTime_t());
    QSqlQuery query("UPDATE cover SET iTimeStamp = "+QString::number(timeStamp)+", \n"
                    " strProvider = '"+strProvider+"', \n"
                    " strFileName = '"+strCoverFile+"' \n"
                    " WHERE idCover = '"+m_currentWork[ID].toString()+"'",*m_musicDatabase->db());
    query.exec();
  }

  //! process next request
  processSearch();
}
