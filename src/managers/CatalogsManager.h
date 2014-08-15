#ifndef _DATABASE_MANAGER_H_
#define _DATABASE_MANAGER_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtSql/QSqlQuery>

#include <plxFramework/models/ListModel.h>
#include <plxFramework/media/Catalog.h>
#include <plxFramework/database/database.h>

class CSettings;

class CCatalogsManager : public CDatabase
{
  Q_OBJECT
  static CCatalogsManager *INSTANCE;

public:  
  ListModel*  m_catalogs;
  CCatalogsManager(QObject* parent = 0);
  static CCatalogsManager* instance();

  virtual bool connect(QString connection){return CDatabase::connect(connection);}
  virtual bool open(){return CDatabase::open();}
  virtual void close(){CDatabase::close();}

signals:
  void settingsChanged();
  void storageDirChanged();
  void catalogsChanged();

public slots:
  ListModel* catalogs();
  CCatalog* currentCatalog();

  int loadCatalogs();
  void setCurrentCatalog(CCatalog* catalog);
  void setCurrentCatalog(int idCatalog);
  int addCatalog(const QString& strName, const QString& strSource, bool autoRebuild = false);
  int deleteCatalog(int idCatalog);

protected:

private:
  CCatalog*   m_currentCatalog;
  QString genID(const QString &strDBName);

  QString m_storageDir;
  CSettings*  m_settings;
  virtual bool createTables();
  virtual void createViews(){};
};

//Q_DECLARE_METATYPE(QList<CCatalog*>)

#endif // _DATABASE_MANAGER_H_
