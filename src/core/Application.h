#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>
#include <QThreadPool>
#include <QQuickView>

#include "../managers/settingsmanager.h"

#include "../managers/CatalogsManager.h"
#include "ThreadManager.h"

#include "CollectionManager.h"
#include "../managers/jukeboxmanager.h"

#include "../views/builderview.h"

class CTheAudioDB;

class CApplication : public QQuickView
{
  Q_OBJECT
  static CApplication* INSTANCE;
  static QThreadPool*  thPool;          // QRunnable manager

public:
  explicit CApplication(QWindow *parent = 0);
  ~CApplication();

  static CApplication* instance() { return INSTANCE; }
  static QThreadPool* threadPool() { return thPool; }

  Q_PROPERTY(CBuilderView* builderView READ builderView NOTIFY builderViewChanged)
  CBuilderView* builderView() { return CBuilderView::instance(); }

  Q_PROPERTY(CJukeboxManager* jukeboxManager READ jukeboxManager)
  CJukeboxManager* jukeboxManager() { return CJukeboxManager::instance(); }

  Q_PROPERTY(CCollectionManager* mainModel READ getMainModel CONSTANT)
  CCollectionManager* getMainModel() { return CCollectionManager::instance(); }

  Q_PROPERTY(CCatalogsManager* databaseManager READ databaseManager)
  CCatalogsManager* databaseManager() { return m_catalogsManager->instance(); }

  Q_INVOKABLE void load();
  Q_INVOKABLE void rebuildDatabase();
  Q_INVOKABLE void selectCatalog(int idCatalog);
  Q_INVOKABLE void moveMouse(int x, int y);
  Q_INVOKABLE bool isCatalogSelected();

  void initialize();

  Q_PROPERTY(int progress READ progress WRITE loadingProgress NOTIFY progressChanged)
  int progress() { return m_iProgress; }
  void resetProgress() { m_iProgress = 0; }

  virtual void keyPressEvent(QKeyEvent * e);

private:
  CCatalogsManager* m_catalogsManager;
  CTheAudioDB* adb;
  int m_iProgress;

signals:
  void loaded();
  void loading();
  void progressChanged(int);

  void builderViewChanged();

private slots:
  void loadViews();
  void setUp();
  void loadingProgress(int i);
};

#endif // APPLICATION_H
