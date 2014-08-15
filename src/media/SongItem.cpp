#include "SongItem.h"

CSongItem::CSongItem(QObject* parent): ListItem(parent)
{

}

QHash<int, QByteArray> CSongItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]       = "id";
  names[FIELD_TRACK]    = "track";
  names[FIELD_TITLE]    = "title";
  names[FIELD_LENGTH]   = "length";
  names[FIELD_GENRE]    = "genre";
  names[FIELD_LABEL]    = "label";
  names[FIELD_YEAR]     = "year";
  names[FIELD_FILEPATH] = "filePath";
  return names;
}
