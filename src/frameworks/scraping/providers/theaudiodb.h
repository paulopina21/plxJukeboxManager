#ifndef THEAUDIODB_H
#define THEAUDIODB_H

#include <plxFramework/concurrent/Worker.h>

#define KEY_TADB "58424d43204d6564696120"

class QNetworkReply;
class CFanartItem;

enum RequestType
{
  ARTIST_DETAILS = Qt::UserRole+1,
  ARTIST_ALBUMS,
  ALBUM_DETAILS,
  TRACK_DETAILS
};

class CTheAudioDB: public CWorker
{
  Q_OBJECT
public:
  CTheAudioDB();

  enum Params
  {
    PARAM_ARTIST = Qt::UserRole+1,
    PARAM_ALBUM,
    PARAM_MBID
  };

  virtual int id() const {}
  virtual QHash<int, QByteArray> roleNames() const {}
  virtual void run();

public slots:
  void parseReply(QByteArray bytes);
  void setRequestType(int request);
  void error(QNetworkReply* reply);

signals:
  void fanartFound(CFanartItem* fanart);

private:
  int m_requestType = -1;
};

#endif // THEAUDIODB_H
