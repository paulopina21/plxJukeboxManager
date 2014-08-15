#include "AlbumItem.h"

CAlbumItem::CAlbumItem(QObject *parent): ListItem(parent)
{
}

QHash<int, QByteArray> CAlbumItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]      = "id";
  names[FIELD_ARTIST]  = "artist";
  names[FIELD_TITLE]   = "title";
  names[FIELD_COVER]   = "cover";
  names[FIELD_YEAR]    = "year";
  names[FIELD_FANART]  = "fanart";
  return names;
}
