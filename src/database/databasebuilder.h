#ifndef _DATABASE_BUILDER_H_
#define _DATABASE_BUILDER_H_

#include <QRunnable>
#include <QObject>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QHash>

#include "musicdatabase.h"

//Forward
class CDatabase;

/*******************************************************************************
    DataBaseBuilder
*******************************************************************************/
// Thread that :
//   - parse collection directory 
//   - read track file metada (using Taglib)
//   - write sql database with track information
class DataBaseBuilder :  public QObject, public QRunnable
{
  Q_OBJECT
  public:
    DataBaseBuilder();
    bool isRunning() {return m_bRunning;}

  protected:
    void run();
  
  public slots:
    void rebuildFolder(QStringList folder);
  
  private slots:
    void setFileCount();
    
  private:
    void insertTrack(const QString& filename);
    void updateTrack(const QString& filename);
    void removeTrack(const QString& filename);
    
//    int AddGenre(const QString& name, QSqlDatabase *sqlDB);
//    int AddArtist(const QString& name, QSqlDatabase *sqlDB);
//    int AddAlbum(const QString& name, QString& idArtist, QString& idGenre, QString& sYear, QSqlDatabase *sqlDB);

//    int AddThumb(const QString& filename, QSqlDatabase *sqlDB);
//    int AddFanart(const QString& filename, QString idThumb, QSqlDatabase *sqlDB);
//    int AddCover(const QString& filename, QString idThumb, QSqlDatabase *sqlDB);

//    void cleanUpDatabase(QSqlDatabase *sqlDB);
    
//    void storeCoverArt(const QString& coverFilePath, const QString& trackFilename);
    void CreateThumbnail(const QString& strOutCoverFilePath, const QString& strInCoverFilePath);
    
  private:
    CMusicDatabase*       m_musicDatabase;

    QHash<QString,uint>   dbMediaFiles;
    QStringList           m_foldersList;
    
    bool                  m_bRunning;
    bool                  m_bExit;
    int                   m_iFileCount;

    QString m_storageDir;

  signals:
    void buildingFinished();
    void buildingProgress(int);
};

#endif // _DATABASE_BUILDER_H_
