#include "SettingItem.h"

CSettingItem::CSettingItem(QObject* parent): ListItem(parent)
{
}

QHash<int, QByteArray> CSettingItem::roleNames() const
{
  QHash<int, QByteArray> names;
  names[FIELD_ID]           = "id";
  names[FIELD_CATEGORY]     = "category";
  names[FIELD_NAME]         = "name";
  names[FIELD_TYPE]         = "type";
  names[FIELD_VALUE]        = "value";
  return names;
}
