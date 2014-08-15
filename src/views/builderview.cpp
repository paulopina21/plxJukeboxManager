#include "builderview.h"
#include <plxFramework/media/mediaitem.h>
#include <plxFramework/models/ListModel.h>
#include <plxFramework/application/Settings.h>

#include "../database/musicdatabase.h"

#include "../media/ArtistItem.h"
#include "../media/AlbumItem.h"
#include "../media/SongItem.h"

CBuilderView* CBuilderView::INSTANCE = 0;
CBuilderView::CBuilderView(QObject *parent): CGUIView(parent)
{
  INSTANCE = this;
//  qmlRegisterType<CView>("views", 1, 0, "BuilderView");
  qmlRegisterType<CArtistItem>("media", 1, 0, "ArtistItem");
  qmlRegisterType<CAlbumItem>("media", 1, 0, "AlbumItem");
  qmlRegisterType<CSongItem>("media", 1,0, "SongItem");

  m_artistList = new ListModel( new CArtistItem(), this );
  m_musicDatabase = CMusicDatabase::instance();
  m_musicDatabase->open();

}

void CBuilderView::setArtistName(CArtistItem* pItem, QString strValue)
{
  pItem->setData(CArtistItem::FIELD_NAME, strValue);
  qDebug() << "New artistName: " + strValue;
}

void CBuilderView::setAlbumTitle(CAlbumItem *pItem, QString strValue)
{
  pItem->setData(CAlbumItem::FIELD_TITLE, strValue);
  qDebug() << "New albumTitle: " + strValue;
}

void CBuilderView::setAlbumYear(CAlbumItem* pItem, QString albumYear)
{
  pItem->setData(CAlbumItem::FIELD_YEAR, albumYear);
  qDebug() << "New albumYear: " + albumYear;
}

void CBuilderView::setSongTrack(CSongItem* pItem, QString strValue)
{
  pItem->setData(CSongItem::FIELD_TRACK, strValue);
  qDebug() << "New songTrack: " + strValue;
}

void CBuilderView::setSongTitle(CSongItem *pItem, QString strValue)
{
  pItem->setData(CSongItem::FIELD_TITLE, strValue);
  qDebug() << "New songTitle: " + strValue;
}

void CBuilderView::setSongGenre(CSongItem* pItem, QString strValue)
{
  pItem->setData(CSongItem::FIELD_GENRE, strValue);
  qDebug() << "New songGenre: " + strValue;
}

void CBuilderView::setSongLabel(CSongItem* pItem, QString strValue)
{
  pItem->setData(CSongItem::FIELD_LABEL, strValue);

}

ListModel* CBuilderView::artistMenu()
{
  m_artistList->clear();

  QString fanartPath = CSettings::folder(CSettings::FOLDER_FANARTS);

  QString strSQL = m_musicDatabase->prepareSQL("SELECT idArtist, strArtist, strFanart FROM artistview ORDER BY strArtist");
  qDebug() << strSQL;
  QSqlQuery query(strSQL, *m_musicDatabase->db());

  while (query.next())
  {
    CArtistItem* pItem = new CArtistItem(m_artistList);
    pItem->setData( CArtistItem::FIELD_ID, query.value(0) );
    pItem->setData( CArtistItem::FIELD_NAME, query.value(1) );
    pItem->setData( CArtistItem::FIELD_FANART, fanartPath + query.value(2).toString() );

    m_artistList->appendRow(pItem);
  }

  return m_artistList;
}

ListModel* CBuilderView::albums(int idArtist)
{
  ListModel* albums = new ListModel( new CAlbumItem(), this );
  QString coversPath = CSettings::folder(CSettings::FOLDER_ALBUMS);

  QString strSQL = m_musicDatabase->prepareSQL("SELECT idAlbum, strAlbum, iYear, idArtist, strArtist, strThumb FROM albumview WHERE idArtist = %i ORDER BY strAlbum", idArtist);
  qDebug() << strSQL;
  QSqlQuery query(strSQL, *m_musicDatabase->db());

  while (query.next())
  {
    CAlbumItem* pItem = new CAlbumItem(albums);
    pItem->setData( CAlbumItem::FIELD_ID, query.value(0) );
    pItem->setData( CAlbumItem::FIELD_TITLE, query.value(1) );
    pItem->setData( CAlbumItem::FIELD_YEAR, query.value(2) );
    pItem->setData( CAlbumItem::FIELD_ARTIST, query.value(4) );
    pItem->setData( CAlbumItem::FIELD_COVER, coversPath + query.value(5).toString() );

    albums->appendRow(pItem);
  }

  return albums;
}

//query("CREATE VIEW songview AS "
//      "SELECT song.idSong, song.idArtist, artist.strArtist, song.idAlbum, album.strAlbum, "
//      "iTrack, strTitle, strDuration, "
//      "song.iYear, strGenre, song.ISRC, song.isVideo, "
//      "label.strLabel, "
//      "path.strPath || pfcfile.strFileName AS strPath, "
//      "song.strFileName "
//      "FROM song "
//      "LEFT OUTER JOIN album ON song.idAlbum=album.idAlbum "
//      "LEFT OUTER JOIN artist ON song.idArtist=artist.idArtist "
//      "LEFT OUTER JOIN label ON song.idLabel=label.idLabel "
//      "LEFT OUTER JOIN pfcfile ON album.idPFCFile=pfcfile.idPFCFile "
//      "LEFT OUTER JOIN path ON pfcfile.idPath=path.idPath "
//      "LEFT OUTER JOIN genre ON song.idGenre=genre.idGenre; ");


ListModel*CBuilderView::songs(int idAlbum)
{
  ListModel* lSongs = new ListModel( new CSongItem(), this );

  QString strSQL = m_musicDatabase->prepareSQL("SELECT * FROM songview WHERE idAlbum = %i ORDER BY iTrack", idAlbum);
  QSqlQuery query(strSQL, *m_musicDatabase->db());

  while (query.next())
  {
    CSongItem* pItem = new CSongItem(lSongs);
    pItem->setData( CSongItem::FIELD_ID, query.value("idSong") );
    pItem->setData( CSongItem::FIELD_TRACK, query.value("iTrack") );
    pItem->setData( CSongItem::FIELD_TITLE, query.value("strTitle") );
    pItem->setData( CSongItem::FIELD_LENGTH, query.value("strDuration") );
    pItem->setData( CSongItem::FIELD_LABEL, query.value("strLabel") );
    pItem->setData( CSongItem::FIELD_GENRE, query.value("strGenre") );
    pItem->setData( CSongItem::FIELD_FILEPATH, query.value("strFileName") );
    pItem->setData( CSongItem::FIELD_YEAR, query.value("iYear") );

    lSongs->appendRow(pItem);
  }

  return lSongs;
}
