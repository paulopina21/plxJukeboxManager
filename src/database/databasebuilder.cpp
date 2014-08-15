// local
#include "databasebuilder.h"

// Qt
#include <QtCore>
#include <QImage>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <QtSql/QSqlResult>

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

// taglib
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <taglib/mpegfile.h>

#include <plxFramework/application/Settings.h>
#include <plxFramework/database/database.h>
#include <plxFramework/media/MediaItem.h>
#include "../managers/CatalogsManager.h"

const QStringList audioFilter = QStringList()
    /* Audio */    << "*.mp3"  << "*.ogg" << "*.wav" << "*.flac" << "*.m4a" << "*.aac"
    /* Video */    << "*.mpeg" << "*.mpg" << "*.avi" << "*.mp4"  << "*.m2ts" << "*.mkv" << "*.ts";

/*******************************************************************************
    concurrent fonction
*******************************************************************************/
int countFiles(const QStringList strFoldersList)
{
  qDebug() << "- DataBaseBuilder -> findFileCount ";

  int count = 0;
  
  foreach(const QString& folder, strFoldersList )
  {
    QDir dirlist(folder);
    dirlist.setNameFilters(audioFilter);
    QDirIterator dirIterator(dirlist,QDirIterator::Subdirectories);
    while (dirIterator.hasNext())
    {
      dirIterator.next();
      count++;
    }
  }
  
  return count;
}

//! --------- DataBaseBuilder::setFileCount -------------------------------
void DataBaseBuilder::setFileCount()
{
  QFutureWatcher<int>* watcher = static_cast<QFutureWatcher<int>*>(sender());
  watcher->deleteLater();

  int i = watcher->result();
  qDebug() << "DataBaseBuilder -> Setting file count to " << i;
  
  m_iFileCount = i;
}

/*******************************************************************************
    DataBaseBuilder
*******************************************************************************/
DataBaseBuilder::DataBaseBuilder()
{
  m_musicDatabase = CMusicDatabase::instance();
  m_storageDir = CSettings::folder(CSettings::FOLDER_STORAGE);
  m_bRunning = false;
//  m_musicDatabase->connect("DataBaseBuilder");
}

//! --------- DataBaseBuilder::rebuildFolder -----------------------------------
// User entry point -> add folder to parse
void DataBaseBuilder::rebuildFolder(QStringList folder)
{
  m_foldersList.append(folder);

  m_iFileCount     = -1;

  QFuture<int> future = QtConcurrent::run(&countFiles,m_foldersList);
  QFutureWatcher<int>* watcher = new QFutureWatcher<int>(this);
  watcher->setFuture(future);
  connect(watcher, SIGNAL(finished()), SLOT(setFileCount()));
}

//! --------- DataBaseBuilder::run --------------------------------------------- 
void DataBaseBuilder::run()
{
  m_bRunning = true;
  if (m_foldersList.isEmpty()) return;
  int percent    = 0;
  int idxCount   = 0;


  qDebug() << "DataBaseBuilder -> Starting Database update";

  //    Database db;
  if (!m_musicDatabase->open()) return;

  //! Populate dbAudioFiles map with all tracks from database
  QSqlQuery trackQuery("SELECT strFilename, iTimestamp FROM song;",*m_musicDatabase->db());
  while (trackQuery.next())
  {
    dbMediaFiles.insert(trackQuery.value(0).toString(),trackQuery.value(1).toUInt());
  }

  QFileInfo fileInfo;

  //On SQLite --> it's MUCH faster to have everything in one transaction
  //with only one disk write than to commit every each insert individually
  m_musicDatabase->beginTransaction();

  do
  {
    QDir dirlist(m_foldersList.takeFirst());
    dirlist.setNameFilters(audioFilter);

    QDirIterator dirIterator(dirlist,QDirIterator::Subdirectories);

    // Laureon: Files Search HERE!!
    while (dirIterator.hasNext())
    {
      dirIterator.next();
      fileInfo = dirIterator.fileInfo();

      //! skip non file
      if (!fileInfo.isFile()) continue;
      bool bCanAdd = CMediaItem::isAudio(fileInfo.filePath().toUtf8()) || CMediaItem::isVideo(fileInfo.filePath().toUtf8());

      //! If the file is NOT in database then insert
      if (!dbMediaFiles.contains(fileInfo.filePath().toUtf8()))
      {
        if (bCanAdd)
          insertTrack(fileInfo.absoluteFilePath());
      }
      //! If the file is in database but has another mtime then update it
      else if (dbMediaFiles[fileInfo.filePath().toUtf8()] != fileInfo.lastModified().toTime_t())
      {
        if (bCanAdd)
          updateTrack(fileInfo.absoluteFilePath());
      }

      dbMediaFiles.remove(fileInfo.filePath().toUtf8());

      //! signal progress
      if (m_iFileCount != -1)
      {
        percent = 100 - ((m_iFileCount - ++idxCount) * 100 / m_iFileCount);
        emit buildingProgress(percent);
      }

    } // WHILE dirIterator.hasNext
  } while (m_foldersList.size()>0);

  //! Get files that are in DB but not on harddrive
  QHashIterator<QString, uint> i(dbMediaFiles);
  while (i.hasNext())
  {
    i.next();
    if( CMediaItem::isAudio(i.key()) || CMediaItem::isVideo(i.key()) )
      removeTrack(i.key());
  }

  dbMediaFiles.clear();

  // Check for interprets/albums/genres... that are not used anymore
  m_musicDatabase->CleanUP();
  // Now write all data to the disk
  m_musicDatabase->commitTransaction();
//  m_musicDatabase->close();

  m_bRunning = false;

  qDebug() << "DataBaseBuilder -> end Database update";

  emit buildingFinished();
  emit buildingProgress(100);
}

//! --------- DataBaseBuilder::insertTrack -------------------------------------
//   -     MEDIA::getCoverName(media) to get hash of covername
void DataBaseBuilder::insertTrack(const QString& filename)
{
  qDebug() << "INFO: DataBaseBuilder -> Inserting song :" << filename;

  //! Load and read tag from file
  CMediaItem* pItem = new CMediaItem(CMediaItem::SONG);
  pItem->loadFromFile(filename);

  qDebug() << "INFO: DataBaseBuilder -> Loaded.";
  //! storage localtion
  QString storageLocation = m_storageDir + "/albums/";


  if (m_musicDatabase->addSong(pItem) == -1) return;

  int idArtist  = pItem->artist()->idArtist();
  int idAlbum   = pItem->album()->idAlbum();

  // COVER WORK
  QString  strCoverThumb    = CMediaItem::buildImageName(pItem->artist()->name(), pItem->album()->title(), false);
  QString  strCoverFile     = CMediaItem::findCoverName(pItem->filePath().toUtf8());
  int idCoverThumb  = m_musicDatabase->addThumb(strCoverThumb);
  int idCover       = m_musicDatabase->addCover(strCoverFile, idCoverThumb);

  if (!m_musicDatabase->setAlbumCover(idAlbum, idCover))
  {
    qDebug() << "WARNING: DataBaseBuilder -> Couldn`t set cover for album: " << idAlbum;
  }
  else
    CreateThumbnail(storageLocation + strCoverThumb, strCoverFile);

  // FANART WORK
  QString  strFanartThumb   = CMediaItem::buildImageName(pItem->artist()->name(), "", true);
  QString  strFanartFile    = CMediaItem::buildImageName(pItem->artist()->name(), "", false);
  int idFanartThumb   = m_musicDatabase->addThumb(strFanartThumb);
  int idFanart = m_musicDatabase->addFanart(strFanartFile, idFanartThumb);

  if (!m_musicDatabase->setArtistFanart(idArtist, idFanart))
  {
    qDebug() << "WARNING: DataBaseBuilder -> Couldn`t add fanart for artist: " << idArtist;
  }

}

void DataBaseBuilder::CreateThumbnail(const QString& strOutCoverFilePath, const QString& strInCoverFilePath)
{
  //qDebug() << "- DataBaseBuilder -> recupCoverArtFromDir " << coverFilePath;

  //! check if coverArt already exist
  QFile file(strOutCoverFilePath);
  if(file.exists()) return ;
  
  //    //! search album art into file source directory
  //    const QStringList imageFilesFilter = QStringList() << "album.jpg" << "cover.jpg" << "capa.jpg" << "folder.jpg" << "thumb.jpg";
  //    const QStringList imageFilters = QStringList() << "*.jpg" << "*.png";
  //    QDir sourceDir(QFileInfo(trackFilename).absolutePath());
  
  //    sourceDir.setNameFilters(imageFilters);

  //    QStringList entryList = sourceDir.entryList(imageFilesFilter, QDir::Files, QDir::Name);
  //    if (entryList.size() == 0)
  //      entryList = sourceDir.entryList(imageFilters, QDir::Files, QDir::Size);

  //    while(!entryList.isEmpty()) {
  //! I take the first one (the biggest one)
  //!WARNING simplification WARNING
  // Laureon: Image caching
  QImage image = QImage(strInCoverFilePath);
  //! check if not null image (occur when file is KO)
  if(!image.isNull()) {
    image = image.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //! check if save is OK
    image.save(strOutCoverFilePath, "jpg", -1);
  }
  //    }
}


//! --------- DataBaseBuilder::updateTrack -------------------------------------
void DataBaseBuilder::updateTrack(const QString& filename)
{
  removeTrack(filename);
  insertTrack(filename);
}

//! --------- DataBaseBuilder::removeTrack -------------------------------------
void DataBaseBuilder::removeTrack(const QString& filename)
{
  qDebug() << "- DataBaseBuilder -> Deleting track :" << filename.toStdString().c_str();
  QFileInfo fileInfo(filename);
  QString fname = fileInfo.filePath().toUtf8();
  QString strQuery = m_musicDatabase->prepareSQL("DELETE FROM song WHERE strFilename = '%s' ",m_musicDatabase->formatValue(fname).toStdString().c_str());
  m_musicDatabase->query(strQuery);
//      m_musicDatabase->Query()->exec();
}
