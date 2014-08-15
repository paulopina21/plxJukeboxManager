#ifndef FANARTITEM_H
#define FANARTITEM_H

#include <plxFramework/models/ListItem.h>

class CFanartItem: public ListItem
{
  Q_OBJECT
public:

  enum Roles
  {
    FIELD_ID = Qt::UserRole+1,
    FIELD_ARTIST,
    FIELD_FULL,
    FIELD_THUMB
  };


  CFanartItem(QObject *parent = 0);

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};
#endif // FANARTITEM_H
