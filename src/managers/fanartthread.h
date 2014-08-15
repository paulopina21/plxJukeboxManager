#ifndef FANARTTHREAD_H
#define FANARTTHREAD_H

#include <QThread>
#include <QStringList>
#include <QObject>
#include <QByteArray>

#include <plxFramework/scraping/Provider.h>
#include "../database/musicdatabase.h"

typedef QMap<int,QVariant> tFields;

class FanartThread : public QThread
{
  Q_OBJECT
public:
  explicit FanartThread(QObject *parent = 0);
  ~FanartThread();
  void setExit(bool b) {m_bExit = b;}
  void setRequest(const QString& artist);

protected:
  void run();

private slots:
  void checkDatabase();
  void processSearch();
  void startProvider();
  void searchFinished();
  void notFound(int id);
  void saveFanart(int, QByteArray);

private:
  QList<ProviderParam>  m_workList;
  ProviderParam  m_currentWork;
  int             m_currentRequestId;
  int             m_iMax;
  bool            m_bExit;

  //! perform search on all missing fanart or on specific request
  bool               m_bFullDBSearch;

  QList<iProvider*>  m_providers;
  int                m_currentProviderId;

  CMusicDatabase*    m_musicDatabase;
signals:
  void finished();
  void progress(int);
};

#endif // FANARTTHREAD_H
