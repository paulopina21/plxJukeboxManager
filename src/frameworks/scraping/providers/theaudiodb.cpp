#include "theaudiodb.h"
#include <plxFramework/network/NetworkAccess.h>
//#include "utilities.h"

#include <QtNetwork/QtNetwork>
#include <QJsonDocument>

#include "../../../media/FanartItem.h"
#include <plxFramework/utils/log.h>

//namespace GLOBAL {
//  CNetworkAccess* http();
//}


CTheAudioDB::CTheAudioDB()
{
}

void CTheAudioDB::run()
{
  QString strKey(KEY_TADB);
  QUrlQuery url;

  switch (m_requestType)
  {
//    case ARTIST_DETAILS:
//      url.setQuery("http://www.theaudiodb.com/api/v1/json/"+strKey+"/search.php?");
//      url.addQueryItem("s", data(PARAM_ARTIST).toString());
//    break;
//    case ALBUM_DETAILS:
//      url.setQuery("http://www.theaudiodb.com/api/v1/json/"+strKey+"/searchalbum.php?");
//      url.addQueryItem("s", data(PARAM_ARTIST).toString());
//      url.addQueryItem("s", data(PARAM_ALBUM).toString());
//    break;
  }

  emit started(0);

  CNetworkReply *reply = CNetworkAccess::instance()->get(url.query());

  connect(reply, SIGNAL(data(QByteArray)), this, SLOT(parseReply(QByteArray)));
  connect(reply, SIGNAL(error(QNetworkReply*)), this, SLOT(error(QNetworkReply*)));
}

void CTheAudioDB::parseReply(QByteArray bytes)
{
  QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes.data());
  QJsonObject jsonObj = jsonDoc.object();

  QJsonArray jsonArray = jsonObj.value(QString("artists")).toArray();

  CLog::log(LOG_DEBUG, "TheAudioDB", jsonArray.size() + " artists found");

  for (int c = 0; c < jsonArray.count(); c++)
  {
    QString strArtist       = jsonArray.at(c).toObject().value(QString("strArtist")).toString();
    QString strFanart       = jsonArray.at(c).toObject().value(QString("strArtistFanart")).toString();
    QString strFanartThumb  = jsonArray.at(c).toObject().value(QString("strArtistFanart")).toString() + "/preview";

    QString strFanart2      = jsonArray.at(c).toObject().value(QString("strArtistFanart2")).toString();
    QString strFanart2Thumb = jsonArray.at(c).toObject().value(QString("strArtistFanart2")).toString() + "/preview";

    QString strFanart3      = jsonArray.at(c).toObject().value(QString("strArtistFanart3")).toString();
    QString strFanart3Thumb = jsonArray.at(c).toObject().value(QString("strArtistFanart3")).toString() + "/preview";

    CFanartItem* pFanart  = new CFanartItem();
    CFanartItem* pFanart2 = new CFanartItem();
    CFanartItem* pFanart3 = new CFanartItem();

    pFanart->setData(CFanartItem::FIELD_ARTIST, strArtist);
    pFanart->setData(CFanartItem::FIELD_FULL, strFanart);
    pFanart->setData(CFanartItem::FIELD_THUMB, strFanartThumb);

    pFanart2->setData(CFanartItem::FIELD_ARTIST, strArtist);
    pFanart2->setData(CFanartItem::FIELD_FULL, strFanart2);
    pFanart2->setData(CFanartItem::FIELD_THUMB, strFanart2Thumb);

    pFanart3->setData(CFanartItem::FIELD_ARTIST, strArtist);
    pFanart3->setData(CFanartItem::FIELD_FULL, strFanart3);
    pFanart3->setData(CFanartItem::FIELD_THUMB, strFanart3Thumb);

    emit fanartFound(pFanart);
    emit fanartFound(pFanart2);
    emit fanartFound(pFanart3);
  }

}

void CTheAudioDB::setRequestType(int request)
{
  m_requestType = request;
}

void CTheAudioDB::error(QNetworkReply*reply)
{
  qDebug() << reply->errorString();
}
