#ifndef PROVIDER_H
#define PROVIDER_H

//#include <QString>
#include <plxFramework/concurrent/Worker.h>

class QNetworkReply;

class CProvider: public CWorker
{
  Q_OBJECT
public:
  virtual int id() const = 0;
  virtual QHash<int, QByteArray> roleNames() const = 0;

public slots:
  virtual void run() = 0;

  virtual void setRequest(int type, QVariant data) = 0;

  QString key();
  void setKey(const QString& strKey);

  QString hostname();
  void setHostname(const QString& strHostname);

  void error(QNetworkReply* reply);
private:
  QString m_strKey;
  QString m_strHostname;
};


#endif // PROVIDER_H
