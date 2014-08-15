#include "plexusalbum.h"

#include <plxFramework/media/mediaitem.h>
#include <plxFramework/network/NetworkAccess.h>
#include <plxFramework/utils/log.h>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include <QImage>
#include <plxFramework/application/Settings.h>
//#include "utilities.h"

#include <QtNetwork/QtNetwork>

namespace GLOBAL {
  CNetworkAccess* http();
}

/*******************************************************************************
  CoverProvider_PlexusAlbum
*******************************************************************************/
CPlexusAlbum::CPlexusAlbum(QObject* parent): iProvider(parent)
{
  setName("PlexusAlbum");
  m_APIKey = "Onc3Upon4Time$";
}

//! ----  set_search -----------------------------------------------------------
void CPlexusAlbum::set_search(const QString& artist, const QString& title)
{
  CLog::log(LOG_DEBUG, "PROVIDER", "Set search: "+artist+" - "+title, "PlexusAlbum");

  m_params[ARTIST] = GetLegalUrlName(artist);
  m_params[TITLE]  = GetLegalUrlName(title);
  m_params[COVER]  = CMediaItem::buildImageName(artist,title);
}

//! ----  start_search ---------------------------------------------------------
void CPlexusAlbum::work(int id)
{
  CLog::log(LOG_INFO, "PROVIDER", "Starting search: "+QString::number(id), "PlexusAlbum");
  if (m_params[ARTIST].toString().isEmpty() || m_params[TITLE].toString().isEmpty())
  {
    emit finished(id);
  }
  else
  {
    requestInfo(id);
  }
}


//! ----  slot_send_cover_info_request -----------------------------------------
void CPlexusAlbum::requestInfo(int id)
{
  QString artistName = artist();
  QString albumTitle = title();

  CLog::log(LOG_INFO, "PROVIDER", "Requesting info for: '"+artistName+"' - '"+albumTitle+"'", "PlexusAlbum");

  QUrlQuery url("http://plexusdynamics.com/admin/api/getalbum.php?");
  url.addQueryItem("key", apiKey());
  url.addQueryItem("artist", artistName);
  url.addQueryItem("album", albumTitle);

  CLog::log(LOG_INFO, "PROVIDER", "Request URL: "+url.query(), "PlexusAlbum");
  qDebug() << "Request URL: "+url.query();

  QObject *reply = CNetworkAccess::instance()->get(url.query());
  _listRequests[reply] = id;

  CLog::log(LOG_DEBUG, "PROVIDER", "Connecting Signals", "PlexusAlbum");
  connect(reply, SIGNAL(data(QByteArray)), this, SLOT(parseInfo(QByteArray)));
  connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
}

//! ----  slot_parse_cover_info ------------------------------------------------
void CPlexusAlbum::parseInfo(QByteArray bytes)
{
  CLog::log(LOG_INFO, "PROVIDER", "Parsing info", "PlexusAlbum");

  /*-------------------------------------------------*/
  /* Get id from sender reply                        */
  /* ------------------------------------------------*/
  QObject* reply = qobject_cast<QObject*>(sender());

  if (!reply || !_listRequests.contains(reply))   return;
  const int id = _listRequests.take(reply);

  /*-------------------------------------------------*/
  /* Parse info                                      */
  /* ------------------------------------------------*/
  bool imageFound = false;

  QJsonDocument jsonResponse = QJsonDocument::fromJson(bytes);
  QJsonObject jsonObject = jsonResponse.object();
  QJsonArray covers = jsonObject["media"].toArray();

  for (int c = 0; c < covers.size(); c++) {
    QJsonObject cover = covers[c].toObject();
    bool imageAlreadyPresent = false;

    // image has to be stored
    if (!cover["url"].toString().isEmpty()){

      //Check file existance and size
      QImage coverFile(CSettings::folder(CSettings::FOLDER_ALBUMS) + m_params[COVER].toString());

      if (!coverFile.isNull()){
        //PAULOPINA - #NEEDFIX: GET LOCAL FILE DIMENSIONS
        //if (coverFile.width() >= cover["width"].toInt() && coverFile.height() >= cover["height"].toInt() )
        //  imageAlreadyPresent = true;
      }

      if (!imageAlreadyPresent) {
        QUrl url = cover["url"].toString();
        QObject *reply = CNetworkAccess::instance()->get(url);
        _listRequests[reply] = id;

        qDebug() << ("Cover Found: " +cover["url"].toString());

        connect(reply, SIGNAL(data(QByteArray)), this, SLOT(receivedInfo(QByteArray)));
        connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
        imageFound = true;
        break;//For now, just get first cover
      }
    }
  }

  // Error handling.
//  if(xml.hasError()) {
//    CLog::log(LOG_ERROR, "PROVIDER", "Network error!", "PlexusAlbum");
//    emit finished(id);
//  }

  if(!imageFound) {
    CLog::log(LOG_WARNING, "PROVIDER", "No cover found!", "PlexusAlbum");
    emit notFound(id);
    emit finished(id);
  }
}


//! --  slot_cover_received ----------------------------------------------------
void CPlexusAlbum::receivedInfo(QByteArray bytes)
{
  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit dataReceived(id, bytes);
}



//! --  slot_cover_search_stop -------------------------------------------------
void CPlexusAlbum::stopSearch()
{
  CLog::log(LOG_INFO, "PROVIDER", "Stop search", "PlexusAlbum");

  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit finished(id);
}

const QString CPlexusAlbum::GetLegalUrlName(const QString& strFileName) {
  char illegalChars[] = {'*', '/', '<', '>', '?', '\\', '[', ']', '\'', ' ' };
  QString strLegalFileName = strFileName;
//  QString legalfile;
//  legalfile.contains();

  for (unsigned int c = 0; c < sizeof(illegalChars); c++)
  {
    for (unsigned int i = 0; i < strLegalFileName.size(); i++)
    {
      if (strLegalFileName.at(i) == illegalChars[c])
      {
        strLegalFileName.replace(i,1,'+');
      }
//      else
//        if (strLegalFileName[i] == ' ') {
//          strLegalFileName.replace(i,1,'+');
//        }
    }
  }

  CLog::log(LOG_DEBUG, "PROVIDER", "Get legal URL name: "+strFileName + " => " + strLegalFileName, "PlexusAlbum");
  return strLegalFileName;
}
