#ifndef MUSICDATABASE_H
#define MUSICDATABASE_H

#include <QtSql/QSqlQuery>
#include <QtQml>

#include <plxFramework/database/database.h>
#include <plxFramework/media/mediaitem.h>

#include "../managers/CatalogsManager.h"
//#include "../frameworks/media/Song.h"
//#include "../frameworks/media/Artist.h"
//#include "../frameworks/media/Album.h"

class CSettings;

class CMusicDatabase: public CDatabase
{
  Q_OBJECT
  static CMusicDatabase* INSTANCE;
public:
  CMusicDatabase(QObject* parent = 0);
  static CMusicDatabase* instance();

  virtual bool connect(QString connection);
  virtual bool open();
  virtual void close();

  bool isEmpty();

  // ARTIST -------------------------------------------------------------
  CArtist getArtist(int idArtist);
  //int AddArtist(CArtist& Artist);
  int addArtist(const QString& strArtist, int idFanart = -1);

  //Make a log of played media
  int logMedia(CMediaItem* pItem);

  int getArtist(const QString& strArtist);
  const QString getArtistByID(int idArtist);

  bool setArtistFanart(int idArtist, int idFanart);

  // Albuns
  CAlbum getAlbum(int idAlbum);

//    CSong GetSongByName();
  CMediaItem *GetSongByFileName(const QString& strFileName);
  CMediaItem *GetSongById(int idSong);
  CMediaItem* songByID(int idSong);

  int getAlbum(const QString& strAlbum, int idArtist = -1);
  int addAlbum(const QString& strAlbum, int idArtist = -1);
  int addAlbum(CAlbum& Album, int idArtist = -1);
  bool setAlbumCover(int idAlbum, int idCover);

  int addSong(CMediaItem *pItem, bool bCheck = true, int idAlbum = -1);

  int getGenre(const QString& strGenre);
  int addGenre(const QString& strGenre);

  int getLabel(const QString& strLabel);
  int addLabel(const QString& strLabel);

  int getThumb(const QString& strFilename);
  int addThumb(const QString& strFilename);

  int getFanart(const QString& strFilename);
  int addFanart(const QString& strFilename, int idThumb = -1);

  int getCover(const QString& strFilename);
  int addCover(const QString& strFilename, int idThumb = -1);

  QList<CArtist*> getArtists(bool bFull = false, QObject* parent = 0);

  QList<CAlbum*> getAlbums(CArtist* artist, QObject *parent = 0);
//  QList<CAlbum*> getAlbums(int idArtist, bool bFull = false, QObject* parent = 0);

  QList<CMediaItem*> getSongs(CAlbum* album);
  QList<CMediaItem*> getSongs(int idAlbum, QObject* parent = 0);

  QList<CArtist*> allArtistsView(bool bFull = false, QObject* parent = 0);

  int getArtistsNav(VEC_ARTIST &vecArtists);
  int getAlbumsNav(VEC_ALBUM& vecMediaItens, int idArtist = -1);
  int getSongsNav(VEC_SONG& vecMediaItens, int idAlbum = -1);

  void CleanUP();

public slots:
  int updateArtist(CArtist *pItem);
  int updateAlbum(CAlbum* pItem);
  int updateSong(CSong* pItem);

private:
  QString m_storageDir;
  CSettings*  m_settings;
  CCatalogsManager* m_catalogsManager;

  virtual bool createTables();
  virtual void createViews();

  bool GetArtistsByWhere(const QString& strWhere, VEC_ARTIST& vecArtists);
  bool GetAlbumsByWhere(const QString& strWhere, VEC_ALBUM& vecAlbums);
  bool GetSongsByWhere(const QString& strWhere, VEC_MEDIAITEM &vecSongs);

  QString genID(const QString &strDBName);
  CArtist GetArtistFromQuery(const QSqlQuery& sqlQuery);
  CAlbum GetAlbumFromQuery(const QSqlQuery& sqlQuery);
  CMediaItem *songFromQuery(const QSqlQuery& sqlQuery, QObject* parent = 0);
};

// Fields should be ordered as they
// appear in the songview
enum SongFields
{
  song_idSong=0,
  song_idArtist,
  song_strArtist,
  song_idAlbum,
  song_strAlbum,
  song_iTrack,
  song_strTitle,
  song_strDuration,
  song_iYear,
  song_strGenre,
  song_strISRC,
  song_iVideo,
  song_strLabel,
  song_strPath,
  song_strFileName,
  song_strExtraArtists,
  song_strExtraGenres,
  song_dwFileNameCRC,
  song_iTimesPlayed,
  song_iStartOffset,
  song_iEndOffset,
  song_lastplayed,
  song_rating,
  song_comment,
  song_idGenre,
  song_strThumb,
  song_iKarNumber,
  song_iKarDelay,
  song_strKarEncoding,
  song_iVisible
} ;

// Fields should be ordered as they
// appear in the albumview
enum AlbumFields
{
  album_idAlbum=0,
  album_idArtist,
  album_strArtist,
  album_strAlbum,
  album_iYear,
  album_strGenre,
//  album_strLabel,
  album_GTIN,
  album_strCover,
  album_strThumb
//  album_strFilePath,
//  album_strExtraArtists,
//  album_strExtraGenres,
//  album_idGenre,
//  album_idAlbumInfo,
//  album_strMoods,
//  album_strStyles,
//  album_strThemes,
//  album_strReview,
//  album_strType,
//  album_strThumbURL,
//  album_iRating,
//  album_iVisible
} ;

enum ArtistFields
{
  artist_idArtist=0, // not a typo - we have the primary key @ 0
  artist_strArtist,
  artist_strFanartPath,
  artist_strFanartThumbPath
//  artist_strBorn,
//  artist_strFormed,
//  artist_strGenres,
//  artist_strMoods,
//  artist_strStyles,
//  artist_strInstruments,
//  artist_strBiography,
//  artist_strDied,
//  artist_strDisbanded,
//  artist_strYearsActive,
//  artist_strImage,
//  artist_strFanart
} ;

//enum JukeboxFields
//{
//  jukebox_idAlbum=0,
//  jukebox_strAlbum,
//  jukebox_strArtist,
//  jukebox_strGenre,
//  jukebox_strCover,
//  jukebox_strFanart,
//  jukebox_strFilePath
//} ;



#endif // MUSICDATABASE_H
