#ifndef SETTINGITEM_H
#define SETTINGITEM_H

#include <plxFramework/models/ListItem.h>

class CSettingItem: public ListItem
{
  Q_OBJECT
public:
  CSettingItem(QObject* parent =0);

  enum Roles
  {
    FIELD_ID = Qt::UserRole+1,
    FIELD_CATEGORY,
    FIELD_NAME,
    FIELD_TYPE,
    FIELD_VALUE
  };

  enum Type
  {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOLEAN,
    TYPE_STRING
  };

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};

#endif // SETTINGITEM_H
