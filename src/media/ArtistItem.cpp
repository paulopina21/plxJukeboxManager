#include "ArtistItem.h"

CArtistItem::CArtistItem(QObject *parent): ListItem(parent)
{
}

QHash<int, QByteArray> CArtistItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]      = "id";
  names[FIELD_NAME]  = "name";
  names[FIELD_FANART]  = "fanart";
  return names;
}
