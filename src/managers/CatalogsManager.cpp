#include "CatalogsManager.h"
#include <QtCore>

#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlField>

#include <plxFramework/application/Settings.h>
#include <plxFramework/utils/log.h>

#define DB_REVISION "15";

CCatalogsManager* CCatalogsManager::INSTANCE = 0;
CCatalogsManager::CCatalogsManager(QObject* parent): CDatabase::CDatabase(parent)
{
  INSTANCE = this;

  m_currentCatalog = NULL;
  m_catalogs = new ListModel(new CCatalog(), this);
  m_settings = CSettings::instance();

  QString strDBFileName = m_settings->value("database/filename").toString();

  if (strDBFileName == "")
  {
    strDBFileName = genID("plxAlbumBuilder");
    m_settings->setValue("database/filename", strDBFileName );
  }

  setFileName(QString(CSettings::folder(CSettings::FOLDER_DATABASE) + strDBFileName + ".db"));
  qDebug() << QString(CSettings::folder(CSettings::FOLDER_DATABASE) + strDBFileName + ".db");
  m_storageDir = CSettings::folder(CSettings::FOLDER_STORAGE);
  connect("CATALOGS MANAGER");
}

CCatalogsManager*CCatalogsManager::instance()
{
  if (!INSTANCE) new CCatalogsManager();
  return INSTANCE;
}

QString CCatalogsManager::genID(const QString &strDBName)
{
  if(strDBName.isEmpty())
    return QString();

  return QString(QCryptographicHash::hash(strDBName.toUtf8().constData(), QCryptographicHash::Sha1).toHex());
}

bool CCatalogsManager::createTables()
{
  bool bResult = true;

  bResult = bResult && query("CREATE TABLE catalog ("         \
     "idCatalog integer NOT NULL PRIMARY KEY AUTOINCREMENT,"  \
     "strCatalog text NOT NULL,"                              \
     "strSourcePath text NOT NULL,"                           \
     "strOutputPath text NOT NULL,"                           \
     "iAlbumCount integer NOT NULL DEFAULT 0,"                \
     "iArtistCount integer NOT NULL DEFAULT 0,"               \
     "iSongCount integer NOT NULL DEFAULT 0,"                 \
     "iAutoRebuild integer NOT NULL DEFAULT 0,"               \
     "strDBFileName text NOT NULL);"                          \
  );
  return bResult;
}

int CCatalogsManager::loadCatalogs(){
  int count = 0;

  CLog::log(LOG_INFO, "CATALOGS_MANAGER", "Loading catalogs");

  //Get Catalogs
  QString strSQL = prepareSQL("SELECT idCatalog, strCatalog, strSourcePath, strOutputPath, iAlbumCount, iArtistCount, iSongCount, iAutoRebuild, strDBFileName FROM catalog");
  QSqlQuery query(strSQL, *db());
  if (!query.exec()) return count;

  while(query.next())
  {
    CCatalog* catalog = new CCatalog(this);
    catalog->setData( CCatalog::FIELD_ID          , query.value(0).toString() );
    catalog->setData( CCatalog::FIELD_NAME        , query.value(1).toString() );
    catalog->setData( CCatalog::FIELD_SOURCE      , query.value(2).toString() );
    catalog->setData( CCatalog::FIELD_OUTPUTPATH  , query.value(3).toString() );
    catalog->setData( CCatalog::FIELD_ALBUMCOUNT  , query.value(4).toInt() );
    catalog->setData( CCatalog::FIELD_ARTISTCOUNT , query.value(5).toInt() );
    catalog->setData( CCatalog::FIELD_SONGCOUNT   , query.value(6).toInt() );
    catalog->setData( CCatalog::FIELD_AUTOREBUILD , query.value(7).toInt() );
    catalog->setData( CCatalog::FIELD_DBFILENAME  , query.value(8).toString() );

    CLog::log(LOG_DEBUG, "CATALOGS_MANAGER", "Source: " + catalog->data(CCatalog::FIELD_SOURCE).toString());

    m_catalogs->appendRow(catalog);
    CLog::log(LOG_INFO, "CATALOGS_MANAGER", "Found catalog: " + catalog->data(CCatalog::FIELD_NAME).toString());
    qDebug() << "Found catalog: " + catalog->data(CCatalog::FIELD_ID).toString() + " - " + catalog->data(CCatalog::FIELD_NAME).toString();
    count++;
  }

}

void CCatalogsManager::setCurrentCatalog(CCatalog* catalog){
  m_currentCatalog = catalog;
}

void CCatalogsManager::setCurrentCatalog(int idCatalog){
  int count = m_catalogs->count();
  for (int c = 0; c < count; c++) {
    if (m_catalogs->at(c)->data(CCatalog::FIELD_ID).toInt() == idCatalog )
      m_currentCatalog = static_cast<CCatalog*>( m_catalogs->at(c) );
  }
//  m_currentCatalog = static_cast<CCatalog*>( m_catalogs->at(idCatalog) );

  qDebug() << idCatalog;
  qDebug() << "Selected catalogue: " + m_currentCatalog->data(CCatalog::FIELD_ID).toString() + " - " + m_currentCatalog->data(CCatalog::FIELD_NAME).toString();
}

int CCatalogsManager::deleteCatalog(int idCatalog){
  int count = m_catalogs->count();
  for (int c = 0; c < count; c++) {
    if (m_catalogs->at(c)->data(CCatalog::FIELD_ID).toInt() == idCatalog ){
      //Remove DB file
      QString strDBFileName = m_catalogs->at(c)->data(CCatalog::FIELD_DBFILENAME).toString();
      QFile::remove(QString(CSettings::folder(CSettings::FOLDER_DATABASE) + strDBFileName + ".db"));
      //Remove DB Record
      QString strSQL = prepareSQL("DELETE FROM catalog WHERE idCatalog = %i", idCatalog);
      QSqlQuery query(strSQL,*db());
      //Remove ListItem
      m_catalogs->removeRow(c);
    }
  }
}

int CCatalogsManager::addCatalog(const QString& strName, const QString& strSource, bool autoRebuild)
{
  if (strName == "" || strSource == "")
    return 0;

  QString strDBFileName = genID(strName+strSource);
  QString strSQL = prepareSQL("INSERT INTO catalog (strCatalog, strSourcePath, strOutputPath, iAutoRebuild, strDBFileName) VALUES (UPPER('%s'), '%s', '%s', %i, '%s')",
                                               formatValue(strName).toStdString().c_str(),
                                               formatValue(strSource).toStdString().c_str(),
                                               formatValue(strSource).toStdString().c_str(),
                                               (autoRebuild ? 1 : 0),
                                               formatValue(strDBFileName).toStdString().c_str() );
  qDebug() << strSQL;
  QSqlQuery query(strSQL,*db());
  int dbId = query.lastInsertId().toInt();
  qDebug() << "CATALOG ADDED DBID: " + dbId;

  if (dbId){
    CCatalog* catalog = new CCatalog(this);
    catalog->setData( CCatalog::FIELD_ID          , dbId );
    catalog->setData( CCatalog::FIELD_NAME        , strName );
    catalog->setData( CCatalog::FIELD_SOURCE      , strSource );
    catalog->setData( CCatalog::FIELD_OUTPUTPATH  , strSource );
    catalog->setData( CCatalog::FIELD_ALBUMCOUNT  , 0 );
    catalog->setData( CCatalog::FIELD_ARTISTCOUNT , 0 );
    catalog->setData( CCatalog::FIELD_SONGCOUNT   , 0 );
    catalog->setData( CCatalog::FIELD_AUTOREBUILD , autoRebuild );
    catalog->setData( CCatalog::FIELD_DBFILENAME  , strDBFileName );

    m_catalogs->appendRow(catalog);
    setCurrentCatalog(dbId);
  }

  return dbId;
}

ListModel* CCatalogsManager::catalogs()
{
  return this->m_catalogs;
}

CCatalog* CCatalogsManager::currentCatalog()
{
  return this->m_currentCatalog;
}
