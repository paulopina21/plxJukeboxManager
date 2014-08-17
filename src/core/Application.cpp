#include "Application.h"

#include <plxFramework/media/Catalog.h>
#include <plxFramework/application/Settings.h>
#include <plxFramework/utils/log.h>

#include "../gui/guidialog.h"

#include "../frameworks/scraping/providers/theaudiodb.h"

//#include "../managers/PFCManager.h"
//#include "../utils/Java.h"

CApplication* CApplication::INSTANCE = 0;
QThreadPool* CApplication::thPool = 0;

CApplication::CApplication(QWindow *parent) : QQuickView(parent)
{
  INSTANCE = this;
  setUp();

  thPool = new QThreadPool(this);

  qmlRegisterType<CApplication>("core", 1, 0, "Application");
  qmlRegisterType<CJukeboxManager>("jukebox", 1, 0, "JukeboxManager");
  qmlRegisterType<CCatalogsManager>("core",1,0,"CatalogsManager");
  qmlRegisterType<CBuilderView>("views", 1, 0, "BuilderView");
  qRegisterMetaType<ListModel*>();

  QCoreApplication::setOrganizationName("PleXus Dynamics");
  QCoreApplication::setOrganizationDomain("plexusdynamics.com.br");
  QCoreApplication::setApplicationName("plxAlbumBuilder");
  QCoreApplication::setApplicationVersion("Albatross");

  resetProgress();
  emit loading();
  initialize();
}

CApplication::~CApplication()
{
//  delete CJava::instance();
//  delete m_threadManager;
}

void CApplication::moveMouse(int x, int y){
  QCursor::setPos(x,y);
}

int CApplication::listQRC(){
  QString strDebug;
  strDebug = "Listing Albums";
  qDebug() << strDebug;
//  QResource::registerResource("/Volumes/MAC Externo/Jukebox/AEROSMITH - BIG ONES.rcc");
//  QResource::registerResource("/Volumes/MAC Externo/Jukebox/AEROSMITH - PERMANENT OUTTAKES.rcc");
//  QResource::registerResource("/Volumes/MAC Externo/Jukebox/AEROSMITH - PERMANENT VACATION.rcc");

  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    strDebug += "\n"+it.next();
  }

  //Save PLX file
  QFile file ("..\debug.txt");
  if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
  QTextStream outstream(&file);
  outstream << strDebug;
  file.close();

  qDebug() << strDebug;

//  QFile f(":/AEROSMITH - PERMANENT VACATION/playlist.plx");
//  if (f.open(QFile::ReadOnly | QFile::Text))
//  {
//    QTextStream in(&f);
//    qDebug() << f.size() << in.readAll();
//    f.close();
//  }

}

void CApplication::initialize()
{
  loadingProgress(0);
  m_catalogsManager = CCatalogsManager::instance();
  m_catalogsManager->open();
  m_catalogsManager->loadCatalogs();
  loadingProgress(100);

//  adb = new CTheAudioDB;
//  adb->setRequestType(ARTIST_DETAILS);
//  adb->setData(CTheAudioDB::PARAM_ARTIST,"Madonna");
//  adb->run();

//  CPFCManager* pfcManager = new CPFCManager(this);
//  pfcManager->buildCatalog();
}

bool CApplication::isCatalogSelected()
{
  if (m_catalogsManager->currentCatalog())
    return true;
  else
    return false;
}

void CApplication::selectCatalog(int idCatalog){
  loadingProgress(0);
  m_catalogsManager->setCurrentCatalog(idCatalog);
  CMusicDatabase::instance()->connect("GLOBAL CONNECTION");
  load();
  //loadingProgress(100);
}

void CApplication::load()
{
  if (m_catalogsManager->currentCatalog()){
    loadingProgress(0);
    new CCollectionManager(this);
    new ThreadManager(this);
    new CJukeboxManager(this);

    if ( m_catalogsManager->currentCatalog()->data(CCatalog::FIELD_AUTOREBUILD).toBool() || CMusicDatabase::instance()->isEmpty() )
      rebuildDatabase();
    else{
      loadViews();
      ThreadManager::instance()->startFanartSearch();
      emit ThreadManager::instance()->dbBuildFinished();
    }

//    ThreadManager::instance()->stopThread();
//    connect(ThreadManager::instance(), SIGNAL(fanartSearchProgress(int)), this, SLOT(loadingProgress(int)) );
//    connect(ThreadManager::instance(), SIGNAL(coverSearchProgress(int)), this, SLOT(loadingProgress(int)) );

    loadingProgress(100);
  }
}

void CApplication::rebuildDatabase()
{
  loadingProgress(0);

  qDebug() << "LETS REBUILD!!";

  connect(ThreadManager::instance(), SIGNAL(dbBuildFinished()), this, SLOT(loadViews()) );
  connect(ThreadManager::instance(), SIGNAL(dbBuildProgress(int)), this, SLOT(loadingProgress(int)) );


  if (m_catalogsManager->currentCatalog())
    ThreadManager::instance()->databaseBuild( m_catalogsManager->currentCatalog()->data(CCatalog::FIELD_SOURCE).toString() );
  else
    qDebug() << "NO CATALOG SELECTED";
}

void CApplication::loadingProgress(int i){
  m_iProgress = i;

  if (i == 0)
    emit loading();

  if (i == 100)
    emit loaded();
}

void CApplication::updateSearchProgress(QString strMessage, QString strContent, int progress, int total){
  qDebug() << strMessage << strContent << progress << total;
  CGUIDialog* dialog = new CGUIDialog(rootObject(), this);
  dialog->showSearchProgress(strMessage, strContent, progress, total);
}


void CApplication::showProgressDialog(){
  CGUIDialog* dialog = new CGUIDialog(rootObject(), this);
  dialog->showProgress("Aguarde...");
}

void CApplication::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_Asterisk:
    {
      CGUIDialog* dialog = new CGUIDialog(rootObject(), this);
//      dialog->show("passo o dia passo a noite cheirando suvaco\n"
//                   "o seu peido fede tanto to até suado\n"
//                   "seu chulé ta azendando tô até com azia\n"
//                   "e esse bafo de cavalo? vá escovar na pia!\n"
//                   "IÊEEEEE IÊEEEE\n"
//                   "By: Paulo Pina","E AGORA, UM POEMA =)");
      dialog->show("Plexus Jukebox Manager\n"
                   "\n"
                   "Paulo Pina\n"
                   "31 8477 2246\n"
                   "paulopina@plexusdynamics.com.br\n"
                   "www.plexusjukebox.com.br", "SOBRE");
    }
    break;
    case Qt::Key_Home:
    {
      CGUIDialog* dialog = new CGUIDialog(rootObject(), this);
      dialog->showProgress("Aguarde...");
    }
    break;

    default:
      QQuickView::keyPressEvent(e);
    break;
  }
}


void CApplication::loadViews()
{
  new CBuilderView(this);
  loadingProgress(100);
}

void CApplication::setUp()
{
  CLog::log(LOG_INFO, "APPLICATION", "Starting up!");
  QCoreApplication::setOrganizationName("PleXus Dynamics");
  QCoreApplication::setOrganizationDomain("plexusdynamics.com.br");
  QCoreApplication::setApplicationName("plxAlbumBuilder");
  QCoreApplication::setApplicationVersion("RC1");

#ifdef QT_DEBUG
  CLog::setLevel(LOG_DEBUG);
#endif

//  new CSettings(qApp->applicationDirPath()+"/config.conf", QSettings::IniFormat, this);
//  new CJava();

#ifndef Q_OS_ANDROID
  CSettings::setFolder(CSettings::FOLDER_STORAGE, QDir::homePath() + "/.PleXusDynamics/" + QCoreApplication::applicationName());
#else
  CSettings::setFolder(CSettings::FOLDER_STORAGE, "/data/data/com.plexusdynamics.media.jukebox.pro/files");
#endif

  QString strStorageRoot      = CSettings::folder(CSettings::FOLDER_STORAGE);

  QString strFolderDatabases  = strStorageRoot + "/databases/";
  QString strFolderAlbums     = strStorageRoot + "/albums/";
  QString strFolderFanarts    = strStorageRoot + "/fanarts/";
  QString strTemp             = strStorageRoot + "/temp/";

  CSettings::setFolder(CSettings::FOLDER_DATABASE, strFolderDatabases);
  CSettings::setFolder(CSettings::FOLDER_ALBUMS, strFolderAlbums);
  CSettings::setFolder(CSettings::FOLDER_FANARTS, strFolderFanarts);
  CSettings::setFolder(CSettings::FOLDER_TEMP, strTemp);

  if ( !QDir().mkpath(strFolderDatabases) )
  {
    CLog::log(LOG_FATAL, "APPLICATION", "Can't create dir: "  + strFolderDatabases);
    qApp->quit();
  }

  if ( !QDir().mkpath(strFolderAlbums) )
  {
    CLog::log(LOG_FATAL, "APPLICATION", "Can't create dir: "  + strFolderAlbums);
    qApp->quit();
  }

  if ( !QDir().mkpath(strFolderFanarts) )
  {
    CLog::log(LOG_FATAL, "APPLICATION", "Can't create dir: "  + strFolderFanarts);
    qApp->quit();
  }

  if ( !QDir().mkpath(strTemp) )
  {
    CLog::log(LOG_FATAL, "APPLICATION", "Can't create dir: "  + strTemp);
    qApp->quit();
  }

  CLog::log(LOG_DEBUG, "APPLICATION", "Storage at: "    + strStorageRoot);
  CLog::log(LOG_DEBUG, "APPLICATION", "Databases at: "  + strFolderDatabases);
  CLog::log(LOG_DEBUG, "APPLICATION", "Albums at: "     + strFolderAlbums);
  CLog::log(LOG_DEBUG, "APPLICATION", "Fanarts at: "    + strFolderFanarts);

//  QSurfaceFormat format = this->format();
////    format.setAlphaBufferSize(8);
//  format.setRenderableType(QSurfaceFormat::OpenGL);
//  this->setFormat(format);
//  this->setClearBeforeRendering(true);
  //  this->setSurfaceType(QSurface::OpenGLSurface);
}
