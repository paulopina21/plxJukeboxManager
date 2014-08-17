#include "CollectionManager.h"
#include <QtConcurrent/QtConcurrent>
#include <plxFramework/media/mediaitem.h>
#include <plxFramework/application/Settings.h>
#include <plxFramework/utils/log.h>

#include "Application.h"

CCollectionManager* CCollectionManager::INSTANCE = 0;

CCollectionManager::CCollectionManager(QObject *parent): QObject(parent)
{
  if (INSTANCE) return;
  INSTANCE = this;
  qmlRegisterType<CCollectionManager>("core", 1, 0, "MainModel");

  qmlRegisterType<CArtist>("music", 1, 0, "Artist");
  qmlRegisterType<CAlbum>("music", 1, 0, "Album");
//  qmlRegisterType<CView>("views", 1, 0, "View");

  m_musicDatabase = CMusicDatabase::instance();
//  m_musicDatabase->connect("CollectionManager");
  m_musicDatabase->open();

  m_strStorageDir = CSettings::folder(CSettings::FOLDER_STORAGE);
}

QQmlListProperty<CArtist> CCollectionManager::artists()
{
//  m_musicDatabase->open();
  m_artists = m_musicDatabase->getArtists(false);
//  m_musicDatabase->close();

  return QQmlListProperty<CArtist>(this, m_artists);
}

QQmlListProperty<CArtist> CCollectionManager::fullArtists()
{
//  m_musicDatabase->open();
  m_fullArtists = m_musicDatabase->getArtists(true, this);
//  m_musicDatabase->close();

  return QQmlListProperty<CArtist>(this, m_fullArtists);
}

CArtist *CCollectionManager::getArtist(int index)
{
  m_musicDatabase->open();
  CArtist* artist = new CArtist(this);
  *artist = *m_artists[index];

//  artist->setAlbums(m_musicDatabase->getAlbums(m_artists[index], true));
//  m_musicDatabase->close();

  return artist;
}

QQmlListProperty<CArtist> CCollectionManager::getArtists()
{
//  m_musicDatabase->open();
  QList<CArtist*> list = m_musicDatabase->getArtists();
//  m_musicDatabase->close();

  return QQmlListProperty<CArtist>(this, list);
}

//QQmlListProperty<CAlbum> CCollectionManager::albums()
//{
//  m_albums = m_musicDatabase->getAlbums(0);
//  return QQmlListProperty<CAlbum>(this, m_albums);
//}

//QQmlListProperty<CAlbum> CCollectionManager::getAlbums(int idArtist)
//{
////  m_musicDatabase->open();
////  m_albums = m_musicDatabase->getAlbums(idArtist);
////  m_musicDatabase->close();

//  return QQmlListProperty<CAlbum>(this, m_albums);
//}

void CCollectionManager::addArtist(CArtist *artist)
{
  m_artists << artist;
  emit artistsChanged();
}

void CCollectionManager::addAlbum(CAlbum *album)
{
  m_albums << album;
  emit albumsChanged();
}

bool CCollectionManager::isCollectionEmpty()
{
  bool bResult = true;
  if (CCatalogsManager::instance()->currentCatalog()){
    m_musicDatabase->open();
    bResult = m_musicDatabase->isEmpty();
  }

  return bResult;
}

int CCollectionManager::saveArtist(CArtist *artist)
{
//  m_musicDatabase->open();
  int iResult = m_musicDatabase->updateArtist(artist);
//  m_musicDatabase->close();
  return iResult;
}

int CCollectionManager::saveAlbum(CAlbum *album)
{
//  m_musicDatabase->open();
  int iResult = m_musicDatabase->updateAlbum(album);
//  m_musicDatabase->close();
  return iResult;
}

int CCollectionManager::saveSong(CSong *song)
{
//  m_musicDatabase->open();
  int iResult = m_musicDatabase->updateSong(song);
//  m_musicDatabase->close();
  return iResult;
}

void CCollectionManager::fullArtistsClean()
{
  m_fullArtists.clear();
}

void CCollectionManager::loadAllArtistView()
{
//  CView::instance()->load(ALLARTISTSVIEW);
}

void CCollectionManager::loadArtistView()
{
//  CView::instance()->load(ARTISTVIEW);
}

int CCollectionManager::listAlbums()
{
  CApplication::instance()->showProgressDialog();

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
  QFile file ("c:\debug.txt");
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

int CCollectionManager::buildAlbums()
{
  CApplication::instance()->showProgressDialog();
  CApplication::instance()->loadingProgress(0);

  removeDir(CSettings::folder(CSettings::FOLDER_TEMP));

  qDebug() << "Building Albums";
  int count = 0;

  QList<CMediaItem*> mediaList;

  QSqlQuery queryCount("SELECT COUNT(idAlbum) FROM albumview", *m_musicDatabase->db());
  queryCount.exec();
  queryCount.first();
  int total = queryCount.value(0).toInt();

  QString strSQL = m_musicDatabase->prepareSQL("SELECT idAlbum, strArtist, strAlbum FROM albumview order by strArtist, strAlbum");
  QSqlQuery query(strSQL, *m_musicDatabase->db());
  if (!query.exec()) return count;

  while(query.next())
  {
    CMediaItem* pAlbum = new CMediaItem(CMediaItem::ALBUM);
    pAlbum->moveToThread(qApp->thread());

    pAlbum = albumBuild(query.value(0).toInt());
    if (pAlbum){
      mediaList.append(pAlbum);
      count++;
    }
    QCoreApplication::processEvents();
    CApplication::instance()->loadingProgress((50 * count) / total);
  }

  count = 0;

  foreach (CMediaItem* mediaItem, mediaList) {
    rccBuild(mediaItem);

    qDebug() << "fanartFile: " + m_fanartFile;
    qDebug() << "coverFile: " + m_coverFile;
    qDebug() << "plxFile: " + m_plxFile;
    qDebug() << "qrcFile: " + m_qrcFile;
    qDebug() << "rccFile: " + m_rccFile;
    count++;
    CApplication::instance()->loadingProgress(((50 * count) / total) + 50);
    QCoreApplication::processEvents();
  }

  qDebug() << QString::number(count)+" Albums Builded";
  CApplication::instance()->loadingProgress(100);
  return count;
}

CMediaItem* CCollectionManager::albumBuild(int idAlbum){
  QString strSQL = m_musicDatabase->prepareSQL("SELECT * FROM albumview WHERE idAlbum = %i", idAlbum);
  QSqlQuery query(strSQL, *m_musicDatabase->db());
  if (!query.exec()) return false;
  if (!query.first()) return false;

  CMediaItem* pMediaItem = new CMediaItem(CMediaItem::ALBUM);
  pMediaItem->moveToThread(qApp->thread());

  CArtist* pArtist = new CArtist();
  pArtist->moveToThread(qApp->thread());
  pArtist->setName(query.value(2).toString());
  pArtist->fanart()->setImage("file:///"+m_strStorageDir + "/fanarts/" + query.value(10).toString() );
  pMediaItem->setArtist(pArtist);

  CAlbum* pAlbum = new CAlbum();
  pAlbum->moveToThread(qApp->thread());
  pAlbum->setIdAlbum(query.value(0));
  pAlbum->setIdArtist(query.value(1));
  pAlbum->setTitle(query.value(3));
  pAlbum->setYear(query.value(4));
  pAlbum->setGenre(query.value(5));
  pAlbum->cover()->setThumb("file:///"+m_strStorageDir + "/albums/" + query.value(8).toString() );
  pAlbum->setSongs(songsBuild(idAlbum));
  pMediaItem->setAlbum(pAlbum);

  return pMediaItem;
}

QList<CSong*> CCollectionManager::songsBuild(int idAlbum){
  QList<CSong*> songs;

  QString strSQL = m_musicDatabase->prepareSQL("SELECT * FROM songview WHERE idAlbum = %i ORDER BY iTrack", idAlbum);
  QSqlQuery query(strSQL, *m_musicDatabase->db());
  if (!query.exec()) return songs;

  while(query.next())
  {
    CSong* pSong = new CSong();
    pSong->moveToThread(qApp->thread());

    pSong->setidSong(query.value(song_idSong));
    pSong->setArtist(query.value(song_strArtist));
    pSong->setTrack(query.value(song_iTrack));
    pSong->setTitle(query.value(song_strTitle).toString());
    pSong->setLenght(query.value(song_strDuration).toString());
    pSong->setYear(query.value(song_iYear));
    pSong->setGenre(query.value(song_strGenre).toString());
    pSong->setIsrc(query.value(song_strISRC).toString());
    pSong->setVideo(query.value(song_iVideo).toInt());
    pSong->setLabel(query.value(song_strLabel).toString());
    pSong->setFilePath(query.value(song_strFileName).toString());

    songs.append(pSong);
  }

  return songs;
}

bool CCollectionManager::xmlBuild(CMediaItem* mediaItem){
  QFileInfo fileInfo;
  QString fileName;

  TiXmlDocument xmlDoc;
  TiXmlDeclaration decl("1.0", "UTF-8", "yes");
  xmlDoc.InsertEndChild(decl);
  TiXmlNode *pMain = NULL;
  pMain = &xmlDoc;

  //Artist
  TiXmlElement artistElement("artist");
  TiXmlNode* artistNode = pMain->InsertEndChild(artistElement);
  XMLUtils::SetString(artistNode,"name", mediaItem->artist()->name());
  fileInfo.setFile(mediaItem->artist()->fanart()->image());
  fileName = fileInfo.fileName();
  XMLUtils::SetString(artistNode,"fanart", fileName);
  m_fanartFile = CSettings::folder(CSettings::FOLDER_FANARTS) + fileName;

  //Album
  TiXmlElement albumElement("album");
  TiXmlNode* albumNode = pMain->InsertEndChild(albumElement);
  XMLUtils::SetString(albumNode,"artist", mediaItem->artist()->name());
  XMLUtils::SetString(albumNode,"title", mediaItem->album()->title());
  XMLUtils::SetString(albumNode,"year", QString::number(mediaItem->album()->year()));
  XMLUtils::SetString(albumNode,"genre", mediaItem->album()->genre());
  XMLUtils::SetString(albumNode,"GTIN", "");
  fileInfo.setFile(mediaItem->album()->cover()->thumb());
  fileName = fileInfo.fileName();
  XMLUtils::SetString(albumNode,"cover", fileName);
  m_coverFile = CSettings::folder(CSettings::FOLDER_ALBUMS) + fileName;

  //Songs
  int count = mediaItem->album()->countSongs();
  for(int i = 0; i < count; i++)
  {
    CSong* song = mediaItem->album()->getSong(i);
    TiXmlElement songElement("song");
    TiXmlNode* songNode = albumNode->InsertEndChild(songElement);
    XMLUtils::SetString(songNode,"artist", song->artist().first());
    QString track;
    XMLUtils::SetString(songNode,"track", track.sprintf("%02d", song->track())); //QString::number(song->track()));
    XMLUtils::SetString(songNode,"title", song->title());
    XMLUtils::SetString(songNode,"length", song->lenght());
    XMLUtils::SetString(songNode,"year", QString::number(song->year()));
    XMLUtils::SetString(songNode,"genre", song->genre());
    XMLUtils::SetString(songNode,"isrc", song->isrc());
    XMLUtils::SetString(songNode,"video", song->video()?"1":"0");
    XMLUtils::SetString(songNode,"label", song->label());
    fileInfo.setFile(song->filepath());
    fileName = fileInfo.fileName();
    XMLUtils::SetString(songNode,"filename", fileName);
  }

  TiXmlPrinter xmlPrinter;
  xmlPrinter.SetIndent( "    " );

  xmlDoc.Accept( &xmlPrinter );
  QString strXmlText = xmlPrinter.CStr();

  //Save PLX file
  QFile file (m_plxFile);
  if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
  QTextStream outstream(&file);
  outstream << strXmlText;
  file.close();

//  qDebug() << strXmlText;
  return true;
}

bool CCollectionManager::qrcBuild(CMediaItem* mediaItem){
  CSong* song = mediaItem->album()->getSong(0);
  QFileInfo fileInfo(song->filepath());

  m_albumDir = fileInfo.dir().dirName();
  if ( !QDir().mkpath(CSettings::folder(CSettings::FOLDER_TEMP) + m_albumDir) )
  {
    CLog::log(LOG_FATAL, "APPLICATION", "Can't create dir: "  + m_albumDir);
    return false;
  }
  m_plxFile = CSettings::folder(CSettings::FOLDER_TEMP) + m_albumDir + "/playlist.plx";
  m_qrcFile = CSettings::folder(CSettings::FOLDER_TEMP) + fileInfo.dir().dirName() + ".qrc";
  m_rccFile = fileInfo.dir().absolutePath() + ".rcc";

  xmlBuild(mediaItem);

  //Copy Cover and Fanart Images
  QString strFanartFileName;
  QString strCoverFileName;

  fileInfo.setFile(m_fanartFile);
  if (fileInfo.exists()){
    strFanartFileName = fileInfo.fileName();
    QFile::copy(m_fanartFile, CSettings::folder(CSettings::FOLDER_TEMP) + m_albumDir + "/" + strFanartFileName);
  }
  else strFanartFileName = "";

  fileInfo.setFile(m_coverFile);
  if (fileInfo.exists()){
    strCoverFileName = fileInfo.fileName();
    QFile::copy(m_coverFile, CSettings::folder(CSettings::FOLDER_TEMP) + m_albumDir + "/" + strCoverFileName);
  }
  else strCoverFileName = "";

  //Save QRC file
  QString strQrcText(
      "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n"
      "<!DOCTYPE RCC><RCC version=\"1.0\">\n"
      "<qresource>\n");
      strQrcText += strFanartFileName == "" ? "" : "    <file>"+m_albumDir+"/"+strFanartFileName+"</file>\n";
      strQrcText += strCoverFileName  == "" ? "" : "    <file>"+m_albumDir+"/"+strCoverFileName+"</file>\n";
      strQrcText += "    <file>"+m_albumDir+"/playlist.plx</file>\n";
      strQrcText += ("</qresource>\n"
      "</RCC>");
  qDebug() << strQrcText;

  QFile file (m_qrcFile);
  if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
  QTextStream outstream(&file);
  outstream << strQrcText;
  file.close();

  return true;
}

bool CCollectionManager::rccBuild(CMediaItem* mediaItem){
  qrcBuild(mediaItem);

  //GENERATE .rcc file
  QString program = QCoreApplication::applicationDirPath() + "/rcc";
  QStringList arguments;
  arguments << "-binary" << "-no-compress" << m_qrcFile << "-o" << m_rccFile;
  qDebug() << program << " " << arguments[0] << " " << arguments[1] << " " << arguments[2] << " " << arguments[3] << " " << arguments[4];

  QProcess rccProcess;
  rccProcess.start(program, arguments);
  if (!rccProcess.waitForFinished())
    return false;

  return true;
}

bool CCollectionManager::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}
