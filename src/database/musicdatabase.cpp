#include "musicdatabase.h"

#include <QDebug>
#include <QtSql/QSqlField>
#include <plxFramework/media/mediaitem.h>
#include <plxFramework/application/Settings.h>
#include "../managers/CatalogsManager.h"

CMusicDatabase* CMusicDatabase::INSTANCE = 0;
CMusicDatabase::CMusicDatabase(QObject *parent): CDatabase::CDatabase(parent)
{
  INSTANCE = this;
  m_settings = CSettings::instance();
  m_catalogsManager = CCatalogsManager::instance();
  m_storageDir = CSettings::folder(CSettings::FOLDER_STORAGE);
  connect("GLOBAL CONNECTION");
}

CMusicDatabase*CMusicDatabase::instance()
{
  if (!INSTANCE) new CMusicDatabase();
  return INSTANCE;
}

QString CMusicDatabase::genID(const QString &strDBName)
{
  if(strDBName.isEmpty())
    return QString();

  return QString(QCryptographicHash::hash(strDBName.toUtf8().constData(), QCryptographicHash::Sha1).toHex());
}

bool CMusicDatabase::connect(QString connection)
{
  QString strDBFileName = m_catalogsManager->currentCatalog()->data(CCatalog::FIELD_DBFILENAME).toString();
  setFileName(QString(CSettings::folder(CSettings::FOLDER_DATABASE) + strDBFileName + ".db"));
  qDebug() << QString(CSettings::folder(CSettings::FOLDER_DATABASE) + strDBFileName + ".db");
  return CDatabase::connect(connection);
}

bool CMusicDatabase::open()
{
  return CDatabase::open();
}

void CMusicDatabase::close()
{
  CDatabase::close();
}

bool CMusicDatabase::isEmpty()
{
  if (!query("SELECT count(*) FROM song")) return true;
  if (!query()->first()) return true;

  qDebug() << "Query size: " << query()->value(0).toInt();
  return (query()->value(0).toInt() <= 0);
}


bool CMusicDatabase::createTables()
{
  qDebug() << "Database: Initializing database structure";
  CDatabase::createTables();
  //  QSqlQuery query(*sqlDb());

  query("CREATE TABLE cover (idCover INTEGER PRIMARY KEY, idThumb INTEGER, strFileName VARCHAR(512), iTimeStamp INTEGER, strProvider VARCHAR(512));");
  query("CREATE TABLE fanart (idFanart INTEGER PRIMARY KEY, idThumb INTEGER, strFileName VARCHAR(512), iTimeStamp INTEGER, strProvider VARCHAR(512));");
  query("CREATE TABLE thumb (idThumb INTEGER PRIMARY KEY, strFileName VARCHAR(512));");

  query("CREATE TABLE path (idPath INTEGER PRIMARY KEY, strPath VARCHAR(512), strHash TEXT);");
  query("CREATE TABLE pfcfile (idPFCFile INTEGER PRIMARY KEY, idPath INTEGER, strFileName VARCHAR(512), strHash TEXT);");

  query("CREATE TABLE label (idLabel INTEGER PRIMARY KEY, strLabel VARCHAR(256));");
  query("CREATE TABLE genre (idGenre INTEGER PRIMARY KEY, strGenre VARCHAR(256));");

  query("CREATE TABLE artist (idArtist INTEGER PRIMARY KEY, idFanart INTEGER, strArtist TEXT);");

  query("CREATE TABLE album (idAlbum INTEGER PRIMARY KEY, idArtist INTEGER, idPFCFile INTEGER, idCover INTEGER, idGenre INTEGER, idLabel INTEGER, strAlbum TEXT, iYear INTEGER, GTIN VARCHAR(13) );");
  query("CREATE TABLE playerlog (idLog INTEGER PRIMARY KEY, strTitle TEXT, strAlbum TEXT, strArtist TEXT, strLabel VARCHAR(256), strDate TEXT, strTime TEXT);");

  query("CREATE TABLE generatedlog (idEntry INTEGER PRIMARY KEY, idActivity INTEGER, strTimeStamp TEXT DEFAULT (datetime('now','localtime')) )\n");

  query("CREATE TABLE song ("           \
        "idSong INTEGER PRIMARY KEY, "  \
        "idAlbum INTEGER, "             \
        "idArtist INTEGER, "            \
        "idGenre INTEGER, "             \
        "idLabel INTEGER, "             \
        "strTitle TEXT, "               \
        "strFilename TEXT, "            \
        "iTrack INTEGER, "              \
        "strDuration VARCHAR(10), "         \
        "iYear INTEGER, "               \
        "ISRC VARCHAR(12), "            \
        "iTimestamp INTEGER, "          \
        "isVideo INTEGER );");

  //  qDebug() << query.exec("CREATE TABLE db_attribute (name VARCHAR(255), value TEXT);");

  //  qDebug() << " ##Database insert database revision";

  //  qDebug() << query.exec("INSERT INTO db_attribute (name, value) values ('version', "+dbRevision+");");
  //  qDebug() << query.exec("INSERT INTO db_attribute (name, value) values ('lastUpdate', 0);");

  createViews();

  //    //! HISTO
  //    query.exec("CREATE TABLE `histo` (" \
  //                 "    `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
  //                 "    `url` TEXT NOT NULL," \
  //                 "    `name` TEXT NOT NULL," \
  //                 "    `date` VARCHAR(50));");


  //     qDebug() << query.exec("CREATE VIEW `view_histo` AS"                    \
  //                 "    SELECT `histo`.`id`,"                                  \
  //                 "           `histo`.`url`,"                                 \
  //                 "           `histo`.`name`,"                                \
  //                 "           `histo`.`date`,"                                \
  //                 "           `tracks`.`id` AS `track_id`,"                   \
  //                 "           `tracks`.`trackname`,"                          \
  //                 "           `tracks`.`number`,"                             \
  //                 "           `tracks`.`length`,"                             \
  //                 "           `tracks`.`playcount`,"                          \
  //                 "           `tracks`.`albumgain`,"                          \
  //                 "           `tracks`.`albumpeakgain`,"                      \
  //                 "           `tracks`.`trackgain`,"                          \
  //                 "           `tracks`.`trackpeakgain`,"                      \
  //                 "           `artists`.`name` AS `artist_name`,"             \
  //                 "           `albums`.`name` AS `album_name`,"               \
  //                 "           `genres`.`genre` AS `genre_name`,"              \
  //                 "           `years`.`year`"                                 \
  //                 "    FROM `histo`"                                          \
  //                 "    LEFT OUTER JOIN `tracks` ON `histo`.`url` = `tracks`.`filename`" \
  //                 "    LEFT JOIN `artists` ON `tracks`.`artist_id` = `artists`.`id`" \
  //                 "    LEFT JOIN `albums` ON `tracks`.`album_id` = `albums`.`id`" \
  //                 "    LEFT JOIN `genres` ON `tracks`.`genre_id` = `genres`.`id`" \
  //                 "    LEFT JOIN `years` ON `tracks`.`year_id` = `years`.`id`;");


  //    //! playlist part
  //    // type playlist (0 = T_DATABASE, 1 = T_FILE, 2 = T_SMART)
  //    query.exec("CREATE TABLE `playlists` (" \
  //                 "    `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
  //                 "    `filename` TEXT NOT NULL," \
  //                 "    `name` TEXT NOT NULL," \
  //                 "    `type` INTEGER," \
  //                 "    `favorite` INTEGER," \
  //                 "    `mtime` INTEGER);");

  //    query.exec("CREATE TABLE `playlist_items` (" \
  //                 "    `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
  //                 "    `url` TEXT NOT NULL," \
  //                 "    `name` TEXT NOT NULL," \
  //                 "    `playlist_id` INTEGER NOT NULL);");

  //     qDebug() << query.exec("CREATE VIEW `view_playlists` AS"                \
  //                 "    SELECT `playlist_items`.`id`,"                         \
  //                 "           `playlist_items`.`url`,"                        \
  //                 "           `playlist_items`.`name`,"                       \
  //                 "           `playlists`.`id` AS `playlist_id`,"             \
  //                 "           `playlists`.`filename` AS `playlist_filename`," \
  //                 "           `playlists`.`name` AS `playlist_name`,"         \
  //                 "           `playlists`.`type` AS `playlist_type`,"         \
  //                 "           `playlists`.`favorite` AS `playlist_favorite`," \
  //                 "           `playlists`.`mtime` AS `playlist_mtime`,"       \
  //                 "           `tracks`.`trackname` AS `track_trackname`,"     \
  //                 "           `tracks`.`number`,"                             \
  //                 "           `tracks`.`length`,"                             \
  //                 "           `tracks`.`playcount`,"                          \
  //                 "           `tracks`.`albumgain`,"                          \
  //                 "           `tracks`.`albumpeakgain`,"                      \
  //                 "           `tracks`.`trackgain`,"                          \
  //                 "           `tracks`.`trackpeakgain`,"                      \
  //                 "           `artists`.`name` AS `artist_name`,"             \
  //                 "           `albums`.`name` AS `album_name`,"               \
  //                 "           `genres`.`genre` AS `genre_name`,"              \
  //                 "           `years`.`year`"                                 \
  //                 "    FROM `playlist_items`"                        \
  //                 "    LEFT JOIN `playlists` ON `playlist_items`.`playlist_id` = `playlists`.`id`" \
  //                 "    LEFT OUTER JOIN `tracks` ON `playlist_items`.`url` = `tracks`.`filename`"
  //                 "    LEFT JOIN `artists` ON `tracks`.`artist_id` = `artists`.`id`" \
  //                 "    LEFT JOIN `albums` ON `tracks`.`album_id` = `albums`.`id`" \
  //                 "    LEFT JOIN `genres` ON `tracks`.`genre_id` = `genres`.`id`" \
  //                 "    LEFT JOIN `years` ON `tracks`.`year_id` = `years`.`id`;");

  //    // type playlist (0 = T_DATABASE, 1 = T_FILE, 2 = T_SMART)
  //    query.exec("CREATE TABLE `smart_playlists` (" \
  //                 "    `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
  //                 "    `name` TEXT NOT NULL," \
  //                 "    `icon` TEXT NOT NULL," \
  //                 "    `rules` TEXT NOT NULL," \
  //                 "    `type` INTEGER," \
  //                 "    `favorite` INTEGER);");
}

void CMusicDatabase::createViews()
{
  query("DROP VIEW IF EXISTS fanartview");
  query("CREATE VIEW fanartview AS\n"
        "SELECT \n"
        "	fanart.idFanart, \n"
        "	artist.strArtist, \n"
        "	fanart.strFileName AS strFanart, \n"
        "	thumb.strFileName AS strThumb, \n"
        "	fanart.strProvider, \n"
        "	fanart.iTimeStamp \n"
        "FROM fanart \n"
        "	LEFT OUTER JOIN thumb ON thumb.idThumb = fanart.idThumb \n"
        " INNER JOIN artist ON artist.idFanart = fanart.idFanart;");

  query("DROP VIEW IF EXISTS coverview");
  query("CREATE VIEW coverview AS \n"
        "SELECT \n"
        "	cover.idCover,\n"
        "	artist.strArtist,\n"
        "	album.strAlbum,\n"
        "	cover.strFileName AS strCover, \n"
        "	thumb.strFileName AS strThumb, \n"
        "	cover.strProvider, \n"
        "	cover.iTimeStamp \n"
        "FROM cover \n"
        "	LEFT OUTER JOIN thumb ON thumb.idThumb = cover.idThumb \n"
        " INNER JOIN album ON album.idCover = cover.idCover \n"
        " INNER JOIN artist ON artist.idArtist = album.idArtist ");

  query("DROP VIEW IF EXISTS artistview");
  query("CREATE VIEW artistview AS \n"
        "SELECT artist.idArtist, artist.strArtist, \n"
        "fanartview.strFanart, fanartview.strThumb, fanartview.iTimeStamp \n"
        "FROM artist \n"
        "LEFT OUTER JOIN fanartview ON fanartview.idFanart = artist.idFanart; ");

  query("DROP VIEW IF EXISTS albumview");
  query("CREATE VIEW albumview AS "
        "SELECT\n"
        "	album.idAlbum,\n"
        "	album.idArtist,\n"
        "	artist.strArtist,\n"
        "	album.strAlbum,\n"
        "	album.iYear,\n"
        "	genre.strGenre,\n"
        "	album.GTIN,\n"
        "	coverview.strCover,\n"
        "	coverview.strThumb,\n"
        "	path.strPath || pfcfile.strFileName AS strPFCPath,\n"
        "	fanartview.strFanart\n"
        "FROM album\n"
        "LEFT OUTER JOIN artist ON album.idArtist = artist.idArtist\n"
        "LEFT OUTER JOIN genre ON album.idGenre = genre.idGenre\n"
        "LEFT OUTER JOIN coverview ON album.idCover = coverview.idCover AND album.strAlbum = coverview.strAlbum\n"
        "LEFT OUTER JOIN fanartview ON artist.idFanart = fanartview.idFanart\n"
        "LEFT OUTER JOIN pfcfile ON album.idPFCFile = pfcfile.idPFCFile\n"
        "LEFT OUTER JOIN path ON pfcfile.idPath = path.idPath;");

  query("DROP VIEW IF EXISTS songview");
  query("CREATE VIEW songview AS "
        "SELECT song.idSong, song.idArtist, artist.strArtist, song.idAlbum, album.strAlbum, "
        "iTrack, strTitle, strDuration, "
        "song.iYear, strGenre, song.ISRC, song.isVideo, "
        "label.strLabel, "
        "path.strPath || pfcfile.strFileName AS strPath, "
        "song.strFileName "
        "FROM song "
        "LEFT OUTER JOIN album ON song.idAlbum=album.idAlbum "
        "LEFT OUTER JOIN artist ON song.idArtist=artist.idArtist "
        "LEFT OUTER JOIN label ON song.idLabel=label.idLabel "
        "LEFT OUTER JOIN pfcfile ON album.idPFCFile=pfcfile.idPFCFile "
        "LEFT OUTER JOIN path ON pfcfile.idPath=path.idPath "
        "LEFT OUTER JOIN genre ON song.idGenre=genre.idGenre; ");
}

int CMusicDatabase::logMedia(CMediaItem *pItem){
  QString strTitle  = pItem->song()->title();
  QString strArtist = pItem->artist()->name();
  QString strAlbum  = pItem->album()->title();
  QString strLabel  = pItem->song()->label();
  QString strDate   = QDate::currentDate().toString("dd/MM/yyyy");
  QString strTime   = QTime::currentTime().toString("HH:mm");

  QString strSQL = prepareSQL("INSERT INTO playerlog (strTitle, strAlbum, strArtist, strLabel, strDate, strTime) VALUES(UPPER('%s'), UPPER('%s'), UPPER('%s'), UPPER('%s'), '%s', '%s')",
                              formatValue(strTitle).toStdString().c_str(), formatValue(strArtist).toStdString().c_str(), formatValue(strAlbum).toStdString().c_str(),
                              formatValue(strLabel).toStdString().c_str(), formatValue(strDate).toStdString().c_str(), formatValue(strTime).toStdString().c_str());

  if (query(strSQL))
    return query()->lastInsertId().toInt();

  return -1;
}

int CMusicDatabase::getArtist(const QString &strArtist)
{
  QString strSQL = prepareSQL("SELECT idArtist FROM artist WHERE strArtist = UPPER('%s') ", formatValue(strArtist).toStdString().c_str());

  // Laureon: TODO: Use GetArtistsByWhere here!!
  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

const QString CMusicDatabase::getArtistByID(int idArtist)
{
  QString strSQL = prepareSQL("SELECT strArtist FROM artist WHERE idArtist = %i ", idArtist);

  // Laureon: TODO: Use GetArtistsByWhere here!!
  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->size() != 0)
    return m_pDS->value(0).toString();

  return "";
}

int CMusicDatabase::addArtist(const QString& strArtist, int idFanart)
{
  int idArtist = getArtist(strArtist);
  if (idArtist != -1) return idArtist;

  QString strSQL = prepareSQL("INSERT INTO artist(strArtist, idFanart) VALUES(UPPER('%s'), %i)", formatValue(strArtist).toStdString().c_str(), idFanart);

  if (query(strSQL))
    return query()->lastInsertId().toInt();

  return -1;
}

bool CMusicDatabase::setArtistFanart(int idArtist, int idFanart)
{
  QString strSQL = prepareSQL("SELECT * FROM artist WHERE idArtist = %i", idArtist);
  if (!query(strSQL)) return false;
  if (query()->size() == 0)
  {
    qDebug() << "MusicDatabase::" << __FUNCTION__ << ": Artist not found: " << idArtist;
    return false;
  }

  strSQL = prepareSQL("UPDATE artist SET idFanart = %i WHERE idArtist = %i", idFanart, idArtist);

  return query(strSQL);
}

int CMusicDatabase::updateArtist(CArtist* pItem)
{
  open();
  QString strSQL;
  int idArtist = pItem->idArtist();
  QString strArtist = pItem->name().toUpper();

  // Check if an artist with that name already exists... if so we need to swap the artist
  int newArtistID = getArtist(strArtist);
  if (newArtistID != -1 && idArtist != newArtistID)
  {
    // Check if there`s any song linked to that artist and change it...
    strSQL = prepareSQL("UPDATE song SET idArtist = %i WHERE idArtist = %i", newArtistID, idArtist);
    if (!query(strSQL)) return -1;

    // Change albums artist
    strSQL = prepareSQL("UPDATE album SET idArtist = %i WHERE idArtist = %i", newArtistID, idArtist);
    if (!query(strSQL)) return -1;

    // Delete old artist
    strSQL = prepareSQL("DELETE FROM artist WHERE idArtist = %i", idArtist);
    if (!query(strSQL)) return -1;

    pItem->setidArtist(newArtistID);
    return newArtistID;
  }
  else
  {
    strSQL = prepareSQL("UPDATE artist SET strArtist = '%s' WHERE idArtist = %i", formatValue(strArtist).toStdString().c_str(),idArtist);
    if (!query(strSQL)) return -1;

    // Laureon: TODO: Change the fanart name to respect the artist name algorithm...
  }

  close();
  return idArtist;
}

int CMusicDatabase::updateAlbum(CAlbum *pItem)
{
//  open();
//  QString strSQL;
//  int idArtist = pItem->idArtist();
//  int idAlbum = pItem->idAlbum();
//  int iYear = pItem->year();
//  int oldYear = iYear;
//  QString strArtist = pItem->artist()->name().toUpper();
//  QString strAlbum  = pItem->title().toUpper();

//  //-----------------------------------------------------------------------------
//  // Check if an artist with that name already exists... if so we need to swap the artist
//  int newArtistID = getArtist(strArtist);
//  if (idArtist != newArtistID)
//  {
//    if (newArtistID == -1)
//    {
//      int idThumb = addThumb(CMediaItem::buildImageName(strArtist,"",true));
//      int idFanart = addFanart(CMediaItem::buildImageName(strArtist,""), idThumb);
//      strSQL = prepareSQL("INSERT INTO artist(strArtist, idFanart) VALUES('%s',%i)", formatValue(strArtist).toStdString().c_str(), idFanart);

//      if (!query(strSQL)) return -1;

//      newArtistID = query()->lastInsertId().toInt();
//    }

//    strSQL = prepareSQL("UPDATE song SET idArtist = %i WHERE idAlbum = %i", newArtistID, idAlbum);
//    if (!query(strSQL)) return -1;

//    strSQL = prepareSQL("UPDATE album SET idArtist = %i WHERE idAlbum = %i", newArtistID, idAlbum);
//    if (!query(strSQL)) return -1;

//    pItem->setIdArtist(newArtistID);
//    idArtist = newArtistID;
//  }

//  //-----------------------------------------------------------------------------
//  // Check if album title was changed
//  int newAlbumID = getAlbum(strAlbum, idArtist);
//  if (idAlbum != newAlbumID)
//  {
//    if (newAlbumID == -1)
//    {
//      //TODO: Quando album nao existe, trocar nome da cover.
//      strSQL = prepareSQL("UPDATE album SET strAlbum = '%s' WHERE idAlbum = %i", formatValue(strAlbum).toStdString().c_str(), idAlbum);

//      if (!query(strSQL)) return -1;

//      newAlbumID = query()->lastInsertId().toInt();
//    }
//    else
//    {
//      strSQL = prepareSQL("UPDATE song SET idAlbum = %i WHERE idAlbum = %i", newAlbumID, idAlbum);

//      if (!query(strSQL)) return -1;

//      newAlbumID = query()->lastInsertId().toInt();

//      strSQL = prepareSQL("DELETE FROM album WHERE idAlbum = %i", idAlbum);
//      if (!query(strSQL)) return -1;
//    }
//  }

//  //-----------------------------------------------------------------------------
//  // Check if year was changed
//  strSQL = prepareSQL("SELECT iYear FROM album WHERE idAlbum = %i ", idAlbum);
//  m_pDS->exec(strSQL);
//  m_pDS->first();
//  if (m_pDS->size() != 0)
//    oldYear = m_pDS->value(0).toInt();

//  if (iYear != oldYear)
//  {
//      strSQL = prepareSQL("UPDATE album SET iYear = %i WHERE idAlbum = %i", iYear, idAlbum);
//      if (!query(strSQL)) return -1;
//  }

//  close();
}

int CMusicDatabase::updateSong(CSong *pItem){
  open();

  QString strSQL;
  int idSong = pItem->idSong();
  int iTrack = pItem->track();
  int oldTrack = iTrack;
  int newGenre = 0;
  QString strTitle = pItem->title();
  QString oldTitle = strTitle;
  QString strGenre = pItem->genre();
  QString oldGenre = strGenre;

  //-----------------------------------------------------------------------------
  // Get Old Values
  strSQL = prepareSQL("SELECT s.iTrack, s.strTitle, g.strGenre FROM song s LEFT JOIN genre g ON g.idGenre = s.idGenre WHERE idSong = %i ", idSong);
  m_pDS->exec(strSQL);
  m_pDS->first();
  if (m_pDS->size() != 0){
    oldTrack = m_pDS->value(0).toInt();
    oldTitle = m_pDS->value(1).toString();
    oldGenre = m_pDS->value(2).toString();
  }

  //-----------------------------------------------------------------------------
  // Check if track was changed
  if (iTrack != oldTrack)
  {
      strSQL = prepareSQL("UPDATE song SET iTrack = %i WHERE idSong = %i", iTrack, idSong);
      if (!query(strSQL)) return -1;
  }

  //-----------------------------------------------------------------------------
  // Check if title was changed
  if (strTitle != oldTitle)
  {
      strSQL = prepareSQL("UPDATE song SET strTitle = '%s' WHERE idSong = %i", formatValue(strTitle).toStdString().c_str(), idSong);
      if (!query(strSQL)) return -1;
  }

  //-----------------------------------------------------------------------------
  // Check if genre was changed
  if (strGenre != oldGenre)
  {
      strSQL = prepareSQL("SELECT idGenre FROM genre WHERE strGenre = '%s'", formatValue(strGenre).toStdString().c_str());
      m_pDS->exec(strSQL);
      m_pDS->first();
      if (m_pDS->size() != 0){
        newGenre = m_pDS->value(0).toInt();
      }
      if (newGenre > 0){
        strSQL = prepareSQL("UPDATE song SET idGenre = %i WHERE idSong = %i", newGenre, idSong);
        if (!query(strSQL)) return -1;
      }
      else{
        strSQL = prepareSQL("INSERT INTO genre(strGenre) VALUES('%s')", formatValue(strGenre).toStdString().c_str());
        if (!query(strSQL)) return -1;
        newGenre = query()->lastInsertId().toInt();
        strSQL = prepareSQL("UPDATE song SET idGenre = %i WHERE idSong = %i", newGenre, idSong);
        if (!query(strSQL)) return -1;
      }

  }

  close();
}


int CMusicDatabase::getGenre(const QString &strGenre)
{
  QString strSQL = prepareSQL("SELECT idGenre FROM genre WHERE strGenre = UPPER('%s') ", formatValue(strGenre).toStdString().c_str());

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addGenre(const QString& strGenre)
{
  int idGenre = getGenre(strGenre);
  if (idGenre != -1) return idGenre;

  QString strSQL = prepareSQL("INSERT INTO genre(strGenre) VALUES(UPPER('%s'))", formatValue(strGenre).toStdString().c_str());

  if (m_pDS->exec(strSQL))
    return m_pDS->lastInsertId().toInt();

  return -1;

}

int CMusicDatabase::getLabel(const QString &strLabel)
{
  QString strSQL = prepareSQL("SELECT idLabel FROM label WHERE strLabel = UPPER('%s') ", formatValue(strLabel).toStdString().c_str());

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addLabel(const QString& strLabel)
{
  int idLabel = getLabel(strLabel);
  if (idLabel != -1) return idLabel;

  QString strSQL = prepareSQL("INSERT INTO label(strLabel) VALUES(UPPER('%s'))", formatValue(strLabel).toStdString().c_str());

  if (m_pDS->exec(strSQL))
    return m_pDS->lastInsertId().toInt();

  return -1;
}

int CMusicDatabase::getAlbum(const QString &strAlbum, int idArtist)
{
  QString strSQL = prepareSQL("SELECT idAlbum FROM album WHERE strAlbum = UPPER('%s') ",formatValue(strAlbum).toStdString().c_str());
  if (idArtist != -1)
    strSQL += prepareSQL(" AND idArtist = %i ", idArtist);

  // Laureon: TODO: Use GetAlbumsByWhere here!!

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addAlbum(const QString &strAlbum, int idArtist)
{
  // Check if exists in database
  int idAlbum = getAlbum(strAlbum,idArtist);
  if (idAlbum != -1) return idAlbum;

  QString strSQL = prepareSQL("INSERT INTO album(strAlbum, idArtist) VALUES(UPPER('%s'),%i)", formatValue(strAlbum).toStdString().c_str(), idArtist);

  if (query(strSQL))
    return query()->lastInsertId().toInt();

  return -1; // Error control...
}

int CMusicDatabase::addAlbum(CAlbum &album, int idArtist)
{
  // Laureon: TODO: FIX: iArtist thing
  //  int iArtist   = addArtist(album.strArtist);
  //  int idGenre   = addGenre(album.strGenre);
  //  int idLabel   = addLabel(album.strLabel);

  //  if (idArtist == -1) return -1; // Laureon: parei

  //  for (VECSONGS::iterator itSong = album.songs.begin(); itSong != album.songs.end(); ++itSong)
  //  {
  //    itSong->idAlbum = album.idAlbum;
  //    itSong->idArtist   = iArtist;
  //    AddSong(*itSong);
  //  }

}

bool CMusicDatabase::setAlbumCover(int idAlbum, int idCover)
{
  QString strSQL = prepareSQL("SELECT * FROM album WHERE idAlbum = %i", idAlbum);
  if (!query(strSQL)) return false;
  if (query()->size() == 0)
  {
    qDebug() << "MusicDatabase::" << __FUNCTION__ << ": Album not found: " << idAlbum;
    return false;
  }

  strSQL = prepareSQL("UPDATE album SET idCover = %i WHERE idAlbum = %i", idCover, idAlbum);
  if (!query(strSQL)) return false;

  return true;
}

//int CMusicDatabase::AddSong(const CSong &song, bool bCheck, int idAlbum)
//{
//  return -1;
//}

int CMusicDatabase::addSong(CMediaItem* pItem, bool bCheck, int idAlbum)
{
  // Laureon: Artist insert
  QString strArtist = pItem->artist()->name();
  int idArtist  = addArtist(strArtist);
  if (idArtist == -1) return -1;

  pItem->getArtist()->setidArtist(idArtist);
  // Laureon: Album insert
  QString strAlbum = pItem->album()->title();
  if (idAlbum == -1)
    idAlbum = addAlbum(strAlbum, idArtist);

  pItem->getAlbum()->setIdAlbum(idAlbum);

  QString strTitle = formatValue(pItem->song()->title());
  QString strFileName = formatValue(pItem->filePath());

  QString strISRC = pItem->song()->isrc();
  int iTrack = pItem->song()->track();
  QString strDuration = pItem->song()->lenght();
  int iYear = pItem->song()->year();
  int iTimestamp = pItem->timestamp();
  int isVideo = (pItem->isVideo() == true?1:0);

  int idGenre   = addGenre(pItem->song()->genre());
  int idLabel   = addLabel(pItem->song()->label());

  QString strSQL = prepareSQL ("INSERT INTO song "
                               "(idAlbum, idArtist, idGenre ,idLabel, strTitle, strFilename, iTrack, strDuration, iYear, ISRC, iTimestamp, isVideo)"
                               "VALUES (%i,%i,%i,%i,UPPER('%s'),'%s', %i,'%s',%i,'%s', %i, %i)",
                               idAlbum,idArtist,idGenre,idLabel, strTitle.toStdString().c_str(), strFileName.toStdString().c_str(),
                               iTrack, strDuration.toStdString().c_str(), iYear, strISRC.toStdString().c_str(), iTimestamp, isVideo
                               );

  // Laureon: TODO: get max year
  QString strSQLAlbum = prepareSQL("UPDATE album SET iYear = %i WHERE idAlbum = %i", iYear, idAlbum);
  query(strSQLAlbum);

  if (query(strSQL))
    return query()->lastInsertId().toInt();

  return -1;
}

int CMusicDatabase::getThumb(const QString &strFilename)
{
  QString strSQL = prepareSQL("SELECT idThumb FROM thumb WHERE strFileName = '%s' ", formatValue(strFilename).toStdString().c_str());

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addThumb(const QString& strFilename)
{
  int idThumb = getThumb(strFilename);
  if (idThumb != -1) return idThumb;

  QString strSQL = prepareSQL("INSERT INTO thumb(strFileName) VALUES('%s')", formatValue(strFilename).toStdString().c_str());

  if (m_pDS->exec(strSQL))
    return m_pDS->lastInsertId().toInt();

  return -1;
}

int CMusicDatabase::getCover(const QString& strFilename)
{
  QString strSQL = prepareSQL("SELECT idCover FROM cover WHERE strFileName = '%s' ", formatValue(strFilename).toStdString().c_str());

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addCover(const QString& strFilename, int idThumb)
{
  int idCheck = getCover(strFilename);
  if (idCheck != -1) return idCheck;

  QString strSQL = prepareSQL("INSERT INTO cover(strFileName, idThumb) VALUES('%s',%i)", formatValue(strFilename).toStdString().c_str(), idThumb);

  if (m_pDS->exec(strSQL))
    return m_pDS->lastInsertId().toInt();

  return -1;
}

QList<CArtist*> CMusicDatabase::getArtists(bool bFull, QObject *parent)
{
  QList<CArtist*> artistsList;
  QSqlQuery query("SELECT * FROM artistview ORDER BY strArtist",*db());
  if (query.exec())
  {
    while(query.next())
    {
      CArtist* pItem = new CArtist(parent);

      pItem->setidArtist(query.value(artist_idArtist));
      pItem->setName(query.value(artist_strArtist));
      pItem->fanart()->setImage(m_storageDir + "/fanarts/" + query.value(artist_strFanartPath).toString() );
      pItem->fanart()->setThumb(m_storageDir + "/fanarts/" + query.value(artist_strFanartThumbPath).toString() );

//      if (bFull)
//        pItem->setAlbums(getAlbums(pItem, parent));

      artistsList << pItem;
    }
  }

  return (artistsList);
}

QList<CAlbum *> CMusicDatabase::getAlbums(CArtist *artist, QObject *parent)
{
  QList<CAlbum*> albumsList;
  QString strSQL = prepareSQL("SELECT * FROM albumview WHERE idArtist = %i ORDER BY strAlbum",artist->idArtist());

  QSqlQuery query(strSQL, *db());
  if (query.exec())
  {
    while(query.next())
    {
      CAlbum* pItem = new CAlbum(parent);

      pItem->setIdAlbum(query.value(album_idAlbum));

//      pItem->setArtist(artist);

      pItem->setTitle(query.value(album_strAlbum));
      pItem->setYear(query.value(album_iYear));

      pItem->cover()->setImage( query.value(album_strCover).toString() );
      pItem->cover()->setThumb( query.value(album_strThumb).toString() );

      albumsList << pItem;
    }
  }

  return (albumsList);

//  return getAlbums(artist->idArtist(), bFull, artist);
}

//QList<CAlbum*> CMusicDatabase::getAlbums(int idArtist, bool bFull, QObject *parent)
//{
//  QList<CAlbum*> albumsList;
//  QString strSQL = prepareSQL("SELECT * FROM albumview WHERE idArtist = %i ORDER BY strAlbum",idArtist);
//  QSqlQuery query(strSQL, *db());
//  if (query.exec())
//  {
//    while(query.next())
//    {
//      CAlbum* pItem = new CAlbum(parent);

//      pItem->setIdAlbum(query.value(album_idAlbum));

//      pItem->setArtist(query.value(album_strArtist));

//      pItem->setTitle(query.value(album_strAlbum));
//      pItem->setYear(query.value(album_iYear));

//      pItem->cover()->setImage( query.value(album_strCover).toString() );
//      pItem->cover()->setThumb( query.value(album_strThumb).toString() );

//      albumsList << pItem;
//    }
//  }

//  return (albumsList);
//}

QList<CMediaItem *> CMusicDatabase::getSongs(CAlbum *album)
{
  return getSongs(album->idAlbum(), album);
}

QList<CMediaItem *> CMusicDatabase::getSongs(int idAlbum, QObject *parent)
{
  if (!isOpen()) open(); // LAUREON: CRITICAL JUKEBOX OPERATION

  QList<CMediaItem*> mediaList;
  QString strSQL = prepareSQL("SELECT * FROM songview WHERE idAlbum = %i ORDER BY iTrack", idAlbum);
  QSqlQuery query(strSQL, *db());
  if (query.exec())
  {
    while(query.next())
    {
      CMediaItem* mediaItem = new CMediaItem(CMediaItem::SONG, parent);
      CArtist* pArtist = new CArtist(mediaItem);
      CAlbum* pAlbum = new CAlbum(mediaItem);
      CSong* pSong = new CSong(mediaItem);

      pSong->setidSong(query.value(song_idSong));
      pAlbum->setIdAlbum(query.value(song_idAlbum));
      pArtist->setidArtist(query.value(song_idArtist));
      pArtist->setName(query.value(song_strArtist));
      pAlbum->setTitle(query.value(song_strAlbum));
//      pAlbum->setArtist(pArtist);
      pSong->setTrack(query.value(song_iTrack));
      pSong->setTitle(query.value(song_strTitle));
      pSong->setYear(query.value(song_iYear));
      mediaItem->setFile(query.value(song_strFileName).toString());

      pSong->setLenght(query.value(song_strDuration).toString());
      pSong->setLabel(query.value(song_strLabel).toString());
      pSong->setGenre(query.value(song_strGenre).toString());

      //      pItem->cover()->setImage( query.value(album_strCover).toString() );
      //      pItem->cover()->setThumb( query.value(album_strThumb).toString() );
      mediaItem->setArtist(pArtist);
      mediaItem->setAlbum(pAlbum);
      mediaItem->setSong(pSong);

      mediaList << mediaItem;
    }
  }

  return (mediaList);
}

QList<CArtist *> CMusicDatabase::allArtistsView(bool bFull, QObject *parent)
{
  QList<CArtist*> artistsList;
  QSqlQuery query("SELECT * FROM artistview ORDER BY strArtist",*db());
  if (query.exec())
  {
    while(query.next())
    {
      CArtist* pItem = new CArtist(parent);

      pItem->setidArtist(query.value(artist_idArtist));
      pItem->setName(query.value(artist_strArtist));
      pItem->fanart()->setImage(m_storageDir + "/fanarts/" + query.value(artist_strFanartPath).toString() );
      pItem->fanart()->setThumb(m_storageDir + "/fanarts/" + query.value(artist_strFanartThumbPath).toString() );

//      if (bFull)
//        pItem->setAlbums(getAlbums(pItem, false));

      artistsList << pItem;
    }
  }

  return (artistsList);
}

int CMusicDatabase::getArtistsNav(VEC_ARTIST &vecArtists)
{
  //  if (query("SELECT * FROM artistview ORDER BY strArtist"))
  //  {
  //    while(query()->next())
  //    {
  //      CArtist* pItem = new CArtist();

  //      pItem->setidArtist(query()->value(artist_idArtist));
  //      pItem->setField(CArtist::NAME, query()->value(artist_strArtist));
  //      //pItem->data[ARTIST::FANART::idFanart]      = 0;
  ////      pItem->data[ARTIST::strFanart] = Query()->value(artist_strFanartPath).toString();
  ////      pItem->data[ARTIST::strThumb]  = Query()->value(artist_strFanartThumbPath).toString();

  //      vecArtists << pItem;
  //    }
  //  }

  //  return query()->size();
}

int CMusicDatabase::getAlbumsNav(VEC_ALBUM &vecMediaItens, int idArtist)
{
  //  QString strQuery = "SELECT * FROM albumview ";

  //  if (idArtist != -1)
  //    strQuery += prepareSQL("where idArtist = %i",idArtist);

  //  if (query(strQuery))
  //  {
  //    while(query()->next())
  //    {
  //      CAlbum* pItem = new CAlbum();

  //      pItem->set(CAlbum::ID, query()->value(album_idAlbum));
  //      pItem->setField(CAlbum::ID_ARTIST, idArtist);
  //      pItem->setField(CAlbum::ARTIST, query()->value(album_strArtist));
  //      pItem->setField(CAlbum::ALBUM, query()->value(album_strAlbum));
  //      pItem->setField(CAlbum::YEAR, query()->value(album_iYear));
  //      pItem->setField(CAlbum::GENRE, query()->value(album_strGenre));
  ////      pItem->setField(CAlbum::strGTIN]  = Query()->value(album_strGTIN).toString();
  ////      pItem->setField(CAlbum::strCover]  = Query()->value(album_strCover).toString();
  ////      pItem->setField(CAlbum::strThumb]  = Query()->value(album_strThumb).toString();

  //      vecMediaItens.push_back(pItem);
  //    }
  //  }
  //  return query()->size();
}

int CMusicDatabase::getFanart(const QString &strFilename)
{
  QString strSQL = prepareSQL("SELECT idFanart FROM fanart WHERE strFileName = '%s' ", formatValue(strFilename).toStdString().c_str());

  m_pDS->exec(strSQL);
  m_pDS->first();

  if (m_pDS->value(0).toInt() != 0)
    return m_pDS->value(0).toInt();

  return -1;
}

int CMusicDatabase::addFanart(const QString& strFilename, int idThumb)
{
  int idCheck = getFanart(strFilename);
  if (idCheck != -1) return idCheck;

  QString strSQL = prepareSQL("INSERT INTO fanart(strFileName, idThumb) VALUES('%s',%i)", formatValue(strFilename).toStdString().c_str(), idThumb);

  if (m_pDS->exec(strSQL))
    return m_pDS->lastInsertId().toInt();

  return -1;
}


void CMusicDatabase::CleanUP()
{
  // Laureon: TODO: Make it work
  // PauloPina: I did it works ;)
  query("DELETE FROM album WHERE idAlbum NOT IN (SELECT DISTINCT idAlbum FROM song);");
  query("DELETE FROM cover WHERE idCover NOT IN (SELECT DISTINCT idCover FROM album);");
  query("DELETE FROM thumb WHERE idThumb NOT IN (SELECT DISTINCT idThumb FROM cover) AND idThumb NOT IN (SELECT DISTINCT idThumb FROM fanart);");

  query("DELETE FROM artist WHERE idArtist NOT IN (SELECT DISTINCT idArtist FROM song);");
  query("DELETE FROM fanart WHERE idFanart NOT IN (SELECT DISTINCT idFanart FROM artist);");

  query("DELETE FROM genre WHERE idGenre NOT IN (SELECT DISTINCT idGenre FROM song);");
  query("DELETE FROM label WHERE idLabel NOT IN (SELECT DISTINCT idLabel FROM song);");
}


CMediaItem* CMusicDatabase::songFromQuery(const QSqlQuery &sqlQuery, QObject *parent)
{
  CMediaItem* pMediaItem = new CMediaItem(CMediaItem::SONG, parent);
  CArtist* pArtist = new CArtist(pMediaItem);
  CAlbum* pAlbum = new CAlbum(pMediaItem);
  CSong* pSong = new CSong(pMediaItem);


  pSong->setidSong( sqlQuery.value(song_idSong) );
  pArtist->setName( sqlQuery.value(song_strArtist) );
  pAlbum->setTitle( sqlQuery.value(song_strAlbum) );
//  pAlbum->setArtist( pArtist );
  pSong->setTrack( sqlQuery.value(song_iTrack) );
  pSong->setTitle( sqlQuery.value(song_strTitle) );
  pSong->setLenght( sqlQuery.value(song_strDuration) );
  pSong->setYear( sqlQuery.value(song_iYear) );
  pSong->setGenre( sqlQuery.value(song_strGenre) );
  pSong->setLabel( sqlQuery.value(song_strLabel) );
  pSong->setIsrc( sqlQuery.value(song_strISRC) );
  pMediaItem->setFile( sqlQuery.value(song_strFileName).toString() );

  ////  song.setField( CSong::KARAOKE_NUMBER, sqlQuery.value(song_iKarNumber) );
  ////  song.strKaraokeLyrEncoding = sqlQuery.value(song_strKarEncoding).toString().toStdString();
  ////  song.iKaraokeDelay = sqlQuery.value(song_iKarDelay).toInt();
  //  song->setField( CSong::ISRC, sqlQuery.value(song_strISRC) ); // Laureon: Jukebox System: ISRC Loading from dataset
  ////  song.iVisible = sqlQuery.value(song_iVisible); // Laureon: Added: Song Visibility

  //  if (song.strFileName == "NONE")
  //    song.strFileName.Empty();
  // Get filename with full path
  //  if (!bWithMusicDbPath)
  //  {
  //    QString strExtension = "pfc";//= URIUtils::GetExtension(sqlQuery.value(song_strPath).get_asString());

  ////    if (strExtension.empty())
  ////      strExtension = "pfc";
  //    URIUtils::CreateArchivePath(song.strFileName, strExtension, sqlQuery.value(song_strPath).get_asString(), sqlQuery.value(song_strFileName).get_asString());
  //    //URIUtils::AddFileToFolder(sqlQuery.value(song_strPath).get_asString(), sqlQuery.value(song_strFileName).get_asString(), song.strFileName);
  //  }
  //  else
  //  {
  //    QString strFileName=sqlQuery.value(song_strFileName).get_asString();
  //    QString strExt=URIUtils::GetExtension(strFileName);
  //    song.strFileName.Format("musicdb://3/%ld/%ld%s", sqlQuery.value(song_idAlbum).get_asInt(), sqlQuery.value(song_idSong).get_asInt(), strExt.c_str());
  //  }

  pMediaItem->setArtist(pArtist);
  pMediaItem->setAlbum(pAlbum);
  pMediaItem->setSong(pSong);
  return pMediaItem;
}


bool CMusicDatabase::GetSongsByWhere(const QString &strWhere, VEC_MEDIAITEM &vecSongs)
{
  QString strQuery = "SELECT * FROM songview " +strWhere;

  QSqlQuery qSongs(strQuery, *db());
  qDebug() << "- Database -> " << __FUNCTION__ << qSongs.lastQuery();

  if (qSongs.size() <= 0) return false;

  while (qSongs.next())
    vecSongs.push_back(songFromQuery(qSongs));

  return true;
}


CMediaItem* CMusicDatabase::GetSongByFileName(const QString &strFileName)
{
  VEC_MEDIAITEM songs;
  QString strWhere = prepareSQL("WHERE strFileName = '%s' ", formatValue(strFileName).toStdString().c_str());

  GetSongsByWhere(strWhere,songs);
  if (songs.size() <= 0) {
    CMediaItem* ret = new CMediaItem();
    return ret;
  }
  return songs[0];
}

CMediaItem *CMusicDatabase::GetSongById(int idSong)
{
  VEC_MEDIAITEM songs;
  QString strWhere = prepareSQL("WHERE idSong = '%i' ", idSong);

  GetSongsByWhere(strWhere,songs);
  if (songs.size() <= 0)
  {
    CMediaItem* ret = new CMediaItem();
    return ret;
  }

  return songs[0];
}

CMediaItem *CMusicDatabase::songByID(int idSong)
{
  QString strQuery = prepareSQL("SELECT * FROM songview WHERE idSong = %i",idSong);
  QSqlQuery query(strQuery, *db());

  CMediaItem* pItem = new CMediaItem(CMediaItem::SONG);
  if (!query.exec()) return pItem;

  query.first();
  delete pItem;
  pItem = songFromQuery(query, pItem);


  return pItem;
}
