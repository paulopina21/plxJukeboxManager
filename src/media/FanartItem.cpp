#include "FanartItem.h"

CFanartItem::CFanartItem(QObject* parent): ListItem(parent)
{

}

QHash<int, QByteArray> CFanartItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]       = "id";
  names[FIELD_ARTIST]   = "artist";
  names[FIELD_FULL]     = "full";
  names[FIELD_THUMB]    = "thumb";
  return names;
}
