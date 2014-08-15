#include "htbackdrops.h"

#include <plxFramework/media/mediaitem.h>
#include <plxFramework/network/NetworkAccess.h>
#include <plxFramework/utils/log.h>
#include <QDebug>
//#include "utilities.h"

#include <QtNetwork/QtNetwork>

namespace GLOBAL {
  CNetworkAccess* http();
}

/*******************************************************************************
  FanartProvider_HTBackDrops
*******************************************************************************/
CHTBackdrops::CHTBackdrops(QObject* parent): iProvider(parent)
{
  setName("htbackdrops");
  m_APIKey = "7681a907c805e0670330c694e788e8e8";
}

//! ----  set_search -----------------------------------------------------------
void CHTBackdrops::set_search(const QString& artist)
{
  CLog::log(LOG_DEBUG, "PROVIDER", "Set search: "+artist, "HTBackDrops");

  m_params[ARTIST]  = GetLegalUrlName(artist);
  m_params[FANART]  = CMediaItem::buildImageName(artist,"");
}

//! ----  start_search ---------------------------------------------------------
void CHTBackdrops::work(int id)
{
  CLog::log(LOG_INFO, "PROVIDER", "Starting search: "+QString::number(id), "HTBackDrops");
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
void CHTBackdrops::requestInfo(int id)
{
  //REMOVE ACENTOS E CARACTERES ESPECIAIS
  QString artistName = artist();
  artistName = artistName.normalized(QString::NormalizationForm_D);
  artistName = artistName.replace(QRegExp("[^a-zA-Z0-9\\s]"), "");
  //FIM REMOVE ACENTOS E CARACTERES ESPECIAIS

  CLog::log(LOG_INFO, "PROVIDER", "Requesting info for: '"+artistName+"'", "HTBackDrops");

  QUrlQuery url("http://htbackdrops.org/api/"+apiKey()+"/searchXML/?");
  url.addQueryItem("default_operator", "and");
  url.addQueryItem("keywords", artistName);

  QObject *reply = CNetworkAccess::instance()->get(url.query());
  _listRequests[reply] = id;

  CLog::log(LOG_DEBUG, "PROVIDER", "Connecting Signals", "HTBackDrops");
  connect(reply, SIGNAL(data(QByteArray)), this, SLOT(parseInfo(QByteArray)));
  connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
}

//! ----  slot_parse_fanart_info ------------------------------------------------
void CHTBackdrops::parseInfo(QByteArray bytes)
{
  CLog::log(LOG_INFO, "PROVIDER", "Parsing info", "HTBackDrops");

  /*-------------------------------------------------*/
  /* Get id from sender reply                        */
  /* ------------------------------------------------*/
  QObject* reply = qobject_cast<QObject*>(sender());

  if (!reply || !_listRequests.contains(reply))   return;
  const int id = _listRequests.take(reply);

  /*-------------------------------------------------*/
  /* Parse info                                      */
  /* ------------------------------------------------*/
  QXmlStreamReader xml(bytes);
  bool imageFound = false;

  //! Xml reading loop
  while(!xml.atEnd() && !xml.hasError())
  {
    QXmlStreamReader::TokenType token = xml.readNext();
    //! search Xml start element

    if(token == QXmlStreamReader::StartElement)
    {
      //! search Xml Image element (size = medium, large, extralarge)
      if(xml.name() == "id")
      {
        //! we have to check if image is already stored
        bool imageAlreadyPresent = false;

        // Laureon: Removed image existance check this isn`t supposed to be here...
        // image has to be stored
        if (!imageAlreadyPresent) {
          QString imageUrl = xml.readElementText();
          if (!imageUrl.isEmpty())
          {
            QUrl url = "http://htbackdrops.org/api/"+apiKey()+"/download/"+imageUrl.toUtf8()+"/fullsize";
            QObject *reply = CNetworkAccess::instance()->get(url);
            _listRequests[reply] = id;

            connect(reply, SIGNAL(data(QByteArray)), this, SLOT(receivedInfo(QByteArray)));
            connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(stopSearch()));
            imageFound = true;
            break;
          }
        }
      } // end Xml image element
    } // end Xml start element
  } // end Xml reading loop

  // Error handling.
  if(xml.hasError()) {
    CLog::log(LOG_ERROR, "PROVIDER", "Network error!", "HTBackDrops");
    emit finished(id);
  }

  if(!imageFound) {
    CLog::log(LOG_WARNING, "PROVIDER", "No fanart found!", "HTBackDrops");
    emit notFound(id);
    emit finished(id);
  }
}


//! --  slot_fanart_received ----------------------------------------------------
void CHTBackdrops::receivedInfo(QByteArray bytes)
{
  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit dataReceived(id, bytes);
}



//! --  slot_fanart_search_stop -------------------------------------------------
void CHTBackdrops::stopSearch()
{
  CLog::log(LOG_INFO, "PROVIDER", "Stop search", "HTBackDrops");

  // Get id from sender reply
  QObject* reply = qobject_cast<QObject*>(sender());
  if (!reply || !_listRequests.contains(reply))   return;

  const int id = _listRequests.take(reply);

  // Send signal for data reception
  emit finished(id);
}

const QString CHTBackdrops::GetLegalUrlName(const QString& strFileName) {
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

  CLog::log(LOG_DEBUG, "PROVIDER", "Get legal URL name: "+strFileName + " => " + strLegalFileName, "HTBackDrops");
  return strLegalFileName;
}
