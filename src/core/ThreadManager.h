#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThreadPool>
#include <QList>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QMap>

//! thread class
class DataBaseBuilder;      // thread for building database for music files

//class CollectionPopulator;  // thread to populate CollectionModel
class CoverThread;          // thread to search cover art from plexus
class FanartThread;          // thread to search fanart from plexus

class PFCGenerator; // thread to generate the PFCFile

enum E_MODEL_TYPE {
  MODEL_COLLECTION = 0,
  MODEL_PLAYLIST   = 1,
  MODEL_ALL        = 2
};
/*******************************************************************************
  Class ThreadManager
*******************************************************************************/
class ThreadManager : public QObject
{
  Q_OBJECT
public:
  explicit ThreadManager(QObject* parent = 0);
  static ThreadManager    *INSTANCE;
  static ThreadManager* instance() { return INSTANCE; }

  ~ThreadManager();

private:
  enum E_THREAD {POPULATOR_C_THREAD, POPULATOR_P_THREAD, COVER_SEARCH_THREAD, FANART_SEARCH_THREAD, PFC_GENERATOR};
  void cancelThread(E_THREAD thread);


public slots:
  void stopThread();

  // Database Builder Thread
  void databaseBuild(const QString& strSource);
  bool isDbRunning();

  // Model Populator Thread
  void populateCollectionModel();

  //PFC Generation thread
  void PFCGenerateCollection();
  void PFCGenerateCollection(const QString& artist, const QString& album);

  // Cover Search thread
  void startCoverSearch();
  void startCoverSearch(const QString& artist, const QString& album);

  // Fanart Search thread
  void startFanartSearch();
  void startFanartSearch(const QString& artist);

private slots:
  void dbBuildProgressChanged(int progress);
  void dbBuildFinish();

  void modelCollectionPopulated();
  void modelCollectionPopulatingChanged(int progress);
  void modelCollectionPopulatingRunning();

  void PFCGeneratorFinished();
  void PFCGeneratorProgress(int progress);
  //    void PFCGenerationRunning();

  void coverSearchFinished();
  void coverSearchProgress(QString strMessage, QString strAlbum, int progress, int total);

  void fanartSearchFinished();
  void fanartSearchProgress(QString strMessage, QString strArtist, int progress, int total);

private:
  QThreadPool            *m_threadPool;          // QRunnable manager
  DataBaseBuilder        *m_databaseBuilder;     // QRunnable

  //    CollectionPopulator    *m_collectionPopulator; // QThread
  CoverThread            *m_coverThread;         // QThread
  FanartThread           *m_fanartThread;        // QThread

  PFCGenerator           *m_PFCGenerator;

  //! messages Id for StatusWidget management
  QMap<QString, uint>    messageIds;

signals:
  void dbBuildFinished();
  void dbBuildProgress(int);
  void modelPopulationFinished(E_MODEL_TYPE type);
};

#endif // THREADMANAGER_H
