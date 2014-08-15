#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include <plxFramework/models/ListItem.h>

class CArtistItem: public ListItem
{
  Q_OBJECT
public:

  enum Roles
  {
    FIELD_ID = Qt::UserRole+1,
    FIELD_NAME,
    FIELD_FANART
  };

  CArtistItem(QObject *parent = 0);

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};

#endif // ARTISTITEM_H
