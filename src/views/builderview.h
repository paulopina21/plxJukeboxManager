#ifndef BUILDERVIEW_H
#define BUILDERVIEW_H

#include <QObject>
#include "guiview.h"
#include "../managers/CatalogsManager.h"

class CArtistItem;
class CAlbumItem;
class CSongItem;
class CMusicDatabase;

class CBuilderView : public CGUIView
{
  Q_OBJECT
  Q_ENUMS(List)
  static CBuilderView* INSTANCE;
public:

  enum List
  {
    ArtistList = 0,
    AlbumList,
    SongList,
    AllArtistList
  };

  explicit CBuilderView(QObject *parent = 0);
  static CBuilderView* instance() { return INSTANCE; }

signals:
  void artistChanged();
  void albumChanged();
  void songsChanged();

public slots:
  virtual void load() {}
  virtual void unload() {}

  void setArtistName(CArtistItem* pItem, QString strValue);
  void setAlbumTitle(CAlbumItem* pItem, QString strValue);
  void setAlbumYear(CAlbumItem* pItem, QString albumYear);
  void setSongTrack(CSongItem* pItem, QString strValue);
  void setSongTitle(CSongItem* pItem, QString strValue);
  void setSongGenre(CSongItem* pItem, QString strValue);
  void setSongLabel(CSongItem* pItem, QString strValue);
      
  ListModel* artistMenu();
  ListModel* artists() { return m_artistList; }
  ListModel* albums(int idArtist);
  ListModel* songs(int idAlbum);

private:
  ListModel*      m_artistList;

  CMusicDatabase* m_musicDatabase;
};

#endif // BUILDERVIEW_H
