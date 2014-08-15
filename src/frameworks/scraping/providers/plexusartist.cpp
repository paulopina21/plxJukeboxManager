#include "plexusartist.h"

#include <plxFramework/media/mediaitem.h>
#include <plxFramework/network/NetworkAccess.h>
#include <plxFramework/utils/log.h>
#include <QDebug>
#include <QtScript/QScriptEngine>
//#include "utilities.h"

#include <QtNetwork/QtNetwork>

namespace GLOBAL {
  CNetworkAccess* http();
}

/*******************************************************************************
  FanartProvider_PlexusArtist
*******************************************************************************/
CPlexusArtist::CPlexusArtist(QObject* parent): iProvider(parent)
{
  setName("PlexusArtist");
  m_APIKey = "Onc3Upon4Time$";
}

//! ----  set_search -----------------------------------------------------------
void CPlexusArtist::set_search(const QString& artist)
{
  CLog::log(LOG_DEBUG, "PROVIDER", "Set search: "+artist, "PlexusArtist");

  m_params[ARTIST]  = GetLegalUrlName(artist);
  m_params[FANART]  = CMediaItem::buildImageName(artist,"");
}

//! ----  start_search ---------------------------------------------------------
void CPlexusArtist::work(int id)
{
  CLog::log(LOG_INFO, "PROVIDER", "Starting search: "+QString::number(id), "PlexusArtist");
  if (m_params[ARTIST].toString().isEmpty())
  {
    emit finished(id);
  }
  else
  {
    requestInfo(id);
  }
}


//! ----  slot_send_fanart_info_request -----------------------------------------
void CPlexusArtist::requestInfo(int id)
{
  //REMOVE ACENTOS E CARACTERES ESPECIAIS
  QString artistName = artist();
  artistName = artistName.normalized(QString::NormalizationForm_D);
  artistName = artistName.replace(QRegExp("[^a-zA-Z0-9\\s]"), "");
  //FIM REMOVE ACENTOS E CARACTERES ESPECIAIS

  CLog::log(LOG_INFO, "PROVIDER", "Requesting info for: '"+artistName+"'", "PlexusArtist");

  QUrlQuery url("http://plexusdynamics.com/admin/api/getartist.php?");
  url.addQueryItem("key", apiKey());
  url.addQueryItem("artist", artistName);

  QObject *reply = CNetworkAccess::instance()->get(url.query());
  _listRequests[reply] = id;

  CLog::log(LOG_DEBUG, "PROVIDER", "Connecting Signals", "PlexusArtist");
  connect(reply, SIGNAL(data(QByteArray)), this, SLOT(parseInfo(QByteArray)));
  connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
}

//! ----  slot_parse_fanart_info ------------------------------------------------
void CPlexusArtist::parseInfo(QByteArray bytes)
{
  CLog::log(LOG_INFO, "PROVIDER", "Parsing info", "PlexusArtist");

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
  QJsonArray fanarts = jsonObject["media"].toArray();

  for (int c = 0; c < fanarts.size(); c++) {
    QJsonObject fanart = fanarts[c].toObject();
    bool imageAlreadyPresent = false;

    // image has to be stored
    if (!fanart["url"].toString().isEmpty())
    {
      //Paulo Pina: Need to implement image existance and status check by new field idProvider.
      if (!imageAlreadyPresent) {
        QUrl url = fanart["url"].toString();
        QObject *reply = CNetworkAccess::instance()->get(url);
        _listRequests[reply] = id;

        connect(reply, SIGNAL(data(QByteArray)), this, SLOT(receivedInfo(QByteArray)));
        connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
        imageFound = true;
        break;//For now, just get first fanart
      }
    }
  }

  // Error handling.
//  if(xml.hasError()) {
//    CLog::log(LOG_ERROR, "PROVIDER", "Network error!", "PlexusArtist");
//    emit finished(id);
//  }

  if(!imageFound) {
    CLog::log(LOG_WARNING, "PROVIDER", "No fanart found!", "PlexusArtist");
    emit notFound(id);
    emit finished(id);
  }
}


//! --  slot_fanart_received ----------------------------------------------------
void CPlexusArtist::receivedInfo(QByteArray bytes)
{
  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit dataReceived(id, bytes);
}



//! --  slot_fanart_search_stop -------------------------------------------------
void CPlexusArtist::stopSearch()
{
  CLog::log(LOG_INFO, "PROVIDER", "Stop search", "PlexusArtist");

  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit finished(id);
}

const QString CPlexusArtist::GetLegalUrlName(const QString& strFileName) {
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

  CLog::log(LOG_DEBUG, "PROVIDER", "Get legal URL name: "+strFileName + " => " + strLegalFileName, "PlexusArtist");
  return strLegalFileName;
}
