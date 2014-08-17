#include "ThreadManager.h"

//#include "CollectionModel/collectionpopulator.h"
#include "../database/databasebuilder.h"

#include "../managers/fanartthread.h"
#include "../managers/coverthread.h"

//#include "Widget/statuswidget.h"

//#include "PFCGenerator/PFCGenerator.h"
#include "Application.h"

#include <QSignalMapper>
#include <QDebug>

ThreadManager* ThreadManager::INSTANCE = 0;
/*******************************************************************************
 Class ThreadManager
*******************************************************************************/
ThreadManager::ThreadManager(QObject* parent): QObject(parent)
{
  INSTANCE = this;
  m_threadPool = new QThreadPool(this);

  // Qrunnable
  m_databaseBuilder = new DataBaseBuilder();
  m_databaseBuilder->setAutoDelete(false);

  // QThread
  //    m_collectionPopulator = new CollectionPopulator();
  m_coverThread = 0;
  m_fanartThread = 0;

  // PFC
  m_PFCGenerator = 0;

  // connection
  QObject::connect(m_databaseBuilder,SIGNAL(buildingFinished()),this,SLOT(dbBuildFinish()));
  QObject::connect(m_databaseBuilder,SIGNAL(buildingProgress(int)),this,SLOT(dbBuildProgressChanged(int)));
  QObject::connect(m_databaseBuilder,SIGNAL(buildingProgress(int)),this,SIGNAL(dbBuildProgress(int)));

  //    QObject::connect(m_collectionPopulator,SIGNAL(populatingFinished()),this,SLOT(modelCollectionPopulated()));
  //    QObject::connect(m_collectionPopulator,SIGNAL(populatingProgress(int)),this,SLOT(modelCollectionPopulatingChanged(int)));
  //    QObject::connect(m_collectionPopulator, SIGNAL(populatingRunning()), this,SLOT(modelCollectionPopulatingRunning()));
}

ThreadManager::~ThreadManager()
{
  qDebug() << "ThreadManager -> wait to finish ...";
  m_threadPool->waitForDone();
  delete m_databaseBuilder;
  //    delete m_collectionPopulator;
  delete m_threadPool;
//  delete m_PFCGenerator;
}



void ThreadManager::stopThread()
{
  qDebug() << "ThreadManager -> stop running thread";
  //    if(m_collectionPopulator->isRunning())
  //      cancelThread(POPULATOR_C_THREAD);

  if(m_coverThread != 0)
    if(m_coverThread->isRunning())
      cancelThread(COVER_SEARCH_THREAD);

  if(m_fanartThread != 0)
    if(m_fanartThread->isRunning())
      cancelThread(FANART_SEARCH_THREAD);
}

/*******************************************************************************
 Database Scanner Thread
*******************************************************************************/
void ThreadManager::databaseBuild(const QString& strSource)
{
  if ( strSource.isEmpty() )
  {
    qDebug() << "WARNING: DatabaseBuilder -> Couldn't scan catalog: No source provided";
    return;
  }
  if ( m_databaseBuilder->isRunning() ) return;


  qDebug() << "ThreadManager->Database build for: " << strSource;

//  QString source = "/Volumes/LAUREON/Music";

  QStringList sources;
  sources << strSource;
  m_databaseBuilder->rebuildFolder(sources);
  m_threadPool->start(m_databaseBuilder);

  connect(this, SIGNAL(dbBuildFinished()), this, SLOT(startFanartSearch()) );
  //  connect(this, SIGNAL(dbBuildFinished()), this, SLOT(startCoverSearch()) );

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Updating Music Collection") + " (0%)");
//  messageIds.insert("DbUpdate", i);
}

void ThreadManager::dbBuildProgressChanged(int progress)
{
//  QString message = QString(tr("Updating Music Collection") + " (%1%)").arg(QString::number(progress));
//  StatusWidget::instance()->updateProgressMessage( messageIds.value("DbUpdate"), message );
}

void ThreadManager::dbBuildFinish()
{
//  if (messageIds.contains("DbUpdate"))
//    StatusWidget::instance()->stopProgressMessage( messageIds.take("DbUpdate") );

  emit dbBuildFinished();
}

bool ThreadManager::isDbRunning()
{
  return m_databaseBuilder->isRunning();
}



/*******************************************************************************
 Collection Model Population thread
*******************************************************************************/
void ThreadManager::populateCollectionModel()
{
//  qDebug() << "ThreadManager -> start a new collection populator";
//  if(m_collectionPopulator->isRunning()) {
//    cancelThread(POPULATOR_C_THREAD);
//  }

//  m_collectionPopulator->start();

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Loading Music Collection"));
//  messageIds.insert("LoadMusic", i);
}

void ThreadManager::modelCollectionPopulatingChanged(int progress)
{
//  QString message = QString(tr("Loading Music Collection") + " (%1%)").arg(QString::number(progress));
//  StatusWidget::instance()->updateProgressMessage( messageIds.value("LoadMusic"), message );
}

void ThreadManager::modelCollectionPopulated()
{
//  if (messageIds.contains("LoadMusic"))
//    StatusWidget::instance()->stopProgressMessage( messageIds.take("LoadMusic") );

//  emit modelPopulationFinished(MODEL_COLLECTION);
}

void ThreadManager::modelCollectionPopulatingRunning()
{
//  // in order to have graphics Scene updated every 5 seconds
//  emit modelPopulationFinished(MODEL_COLLECTION);
}

// Laureon: PFC
void ThreadManager::PFCGenerateCollection()
{
//  qDebug() << "ThreadManager -> start a new PFC collection generation";

//  if (!m_PFCGenerator) {
//    m_PFCGenerator = new PFCGenerator();

//    connect(m_PFCGenerator,SIGNAL(finished()),this, SLOT(PFCGeneratorFinished()));
//    connect(m_PFCGenerator,SIGNAL(progress(int)),this, SLOT(PFCGeneratorProgress(int)));
//  }


//  if(m_PFCGenerator->isRunning()) {
//    cancelThread(PFC_GENERATOR);
//  }

//  m_PFCGenerator->start();

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Gerando PFCs"));
//  messageIds.insert("PFCGen", i);
}

void ThreadManager::PFCGenerateCollection(const QString &artist, const QString &album)
{
//  qDebug() << "ThreadManager -> start a new PFC single album generation";

//  if(!m_PFCGenerator) {
//    m_PFCGenerator = new PFCGenerator();

//    connect(m_PFCGenerator,SIGNAL(finished()),this, SLOT(PFCGeneratorFinished()));
//  }


//  if(m_PFCGenerator->isRunning()) {
//    cancelThread(PFC_GENERATOR);
//  }

//  m_PFCGenerator->setParams(artist,album);
//  m_PFCGenerator->start();

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Gerando PFC: ") + artist + " - " +album);
//  messageIds.insert("PFCGen", i);
}

void ThreadManager::PFCGeneratorFinished()
{
//  if (messageIds.contains("PFCGen"))
//    StatusWidget::instance()->stopProgressMessage( messageIds.take("PFCGen") );
}

void ThreadManager::PFCGeneratorProgress(int progress)
{
//  QString message = QString(tr("Gerando PFCs") + " (%1%)").arg(QString::number(progress));
//  StatusWidget::instance()->updateProgressMessage( messageIds.value("PFCGen"), message );
}

/*******************************************************************************
  Cover Search Thread
*******************************************************************************/
void ThreadManager::startCoverSearch()
{
  if(!m_coverThread)
  {
    m_coverThread = new CoverThread();

    connect(m_coverThread,SIGNAL(finished()),this,SLOT(coverSearchFinished()));
    connect(m_coverThread,SIGNAL(progress(QString, QString, int, int)), this, SLOT(coverSearchProgress(QString, QString, int, int)));
  }

  if(m_coverThread->isRunning())
    cancelThread(COVER_SEARCH_THREAD);

  m_coverThread->start();

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Fetching Album Cover"));
//  messageIds.insert("CoverUpdate", i);
}

void ThreadManager::startCoverSearch(const QString& artist, const QString& album)
{
  if(!m_coverThread) {
    m_coverThread = new CoverThread();
    connect(m_coverThread,SIGNAL(finished()),this,SLOT(coverSearchFinished()));
  }

  if(m_coverThread->isRunning())
    cancelThread(COVER_SEARCH_THREAD);

  m_coverThread->setRequest(artist, album);
  m_coverThread->start();
}


void ThreadManager::coverSearchFinished()
{
  qDebug() << "ThreadManager: CoverSearch -> Finished cover search";
//  if (messageIds.contains("CoverUpdate"))
//    StatusWidget::instance()->stopProgressMessage( messageIds.take("CoverUpdate") );
}

void ThreadManager::coverSearchProgress(QString strMessage, QString strAlbum, int progress, int total)
{
  CApplication::instance()->updateSearchProgress(strMessage, strAlbum, progress, total);
//  QString message = QString(tr("Fetching Album Cover") + " (%1%)").arg(QString::number(progress));
//  StatusWidget::instance()->updateProgressMessage( messageIds.value("CoverUpdate"), message );
}


/*******************************************************************************
  Fanart Search Thread
*******************************************************************************/
void ThreadManager::startFanartSearch()
{
  if(!m_fanartThread)
  {
    m_fanartThread = new FanartThread();

    connect(m_fanartThread,SIGNAL(finished()), this, SLOT(fanartSearchFinished()));
    connect(m_fanartThread,SIGNAL(progress(QString, QString, int, int)), this, SLOT(fanartSearchProgress(QString, QString, int, int)));
  }

  if(m_fanartThread->isRunning())
    cancelThread(FANART_SEARCH_THREAD);

  m_fanartThread->start();

//  uint i = StatusWidget::instance()->startProgressMessage(tr("Fetching Album Fanart"));
//  messageIds.insert("FanartUpdate", i);
}

void ThreadManager::startFanartSearch(const QString& artist)
{
  if(!m_fanartThread) {
    m_fanartThread = new FanartThread();
    connect(m_fanartThread,SIGNAL(finished()),this, SLOT(fanartSearchFinished()));
  }

  if(m_fanartThread->isRunning())
    cancelThread(FANART_SEARCH_THREAD);

  m_fanartThread->setRequest(artist);
  m_fanartThread->start();
}


void ThreadManager::fanartSearchFinished()
{
  qDebug() << "ThreadManager: FanartSearch -> Finished fanart search";
  startCoverSearch();
//  if (messageIds.contains("FanartUpdate"))
//    StatusWidget::instance()->stopProgressMessage( messageIds.take("FanartUpdate") );
}

void ThreadManager::fanartSearchProgress(QString strMessage, QString strArtist, int progress, int total)
{
  CApplication::instance()->updateSearchProgress(strMessage, strArtist, progress, total);
//  QString message = QString(tr("Fetching Album Fanart") + " (%1%)").arg(QString::number(progress));
//  StatusWidget::instance()->updateProgressMessage( messageIds.value("FanartUpdate"), message );
}



/*******************************************************************************
  Cancel Thread
*******************************************************************************/
void ThreadManager::cancelThread(E_THREAD thread)
{
  switch(thread) {
//    case POPULATOR_C_THREAD:
//      qDebug() << "ThreadManager ->  collection populator isRunning : Stop it !!";
//      m_collectionPopulator->setExit(true);
//      m_collectionPopulator->wait();      // wait end of thread should be fast
//      m_collectionPopulator->setExit(false);
//      if (messageIds.contains("LoadMusic"))
//        StatusWidget::instance()->stopProgressMessage( messageIds.take("LoadMusic") );

//    break;

    case COVER_SEARCH_THREAD:
      m_coverThread->setExit(true);
      m_coverThread->wait();
      m_coverThread->setExit(false);
//      if (messageIds.contains("CoverUpdate"))
//        StatusWidget::instance()->stopProgressMessage( messageIds.take("CoverUpdate") );

    break;

    case FANART_SEARCH_THREAD:
      m_fanartThread->setExit(true);
      m_fanartThread->wait();
      m_fanartThread->setExit(false);
//      if (messageIds.contains("FanartUpdate"))
//        StatusWidget::instance()->stopProgressMessage( messageIds.take("FanartUpdate") );

    break;

    default:break;
  }
}

