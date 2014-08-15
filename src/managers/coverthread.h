#ifndef COVERTHREAD_H
#define COVERTHREAD_H

#include <QThread>
#include <QStringList>
#include <QObject>
#include <QByteArray>

#include <plxFramework/scraping/Provider.h>
#include "../database/musicdatabase.h"

typedef QMap<int,QVariant> tFields;

class CoverThread : public QThread
{
  Q_OBJECT
public:
  explicit CoverThread(QObject *parent = 0);
  ~CoverThread();
  void setExit(bool b) {m_bExit = b;}
  void setRequest(const QString& artist, const QString& title);

protected:
  void run();

private slots:
  void checkDatabase();
  void processSearch();
  void startProvider();
  void searchFinished();
  void notFound(int id);
  void saveCover(int, QByteArray);

private:
  QList<ProviderParam>  m_workList;
  ProviderParam   m_currentWork;
  int             m_currentRequestId;
  int             m_iMax;
  bool            m_bExit;

  //! perform search on all missing Cover or on specific request
  bool               m_bFullDBSearch;

  QList<iProvider*>  m_providers;
  int                m_currentProviderId;

  CMusicDatabase*    m_musicDatabase;
signals:
  void finished();
  void progress(int);
};

#endif // COVERTHREAD_H
