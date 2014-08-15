#ifndef ALBUMITEM_H
#define ALBUMITEM_H

#include <plxFramework/models/ListItem.h>

class CAlbumItem: public ListItem
{
  Q_OBJECT
public:

  enum Roles
  {
    FIELD_ID = Qt::UserRole+1,
    FIELD_ARTIST,
    FIELD_TITLE,
    FIELD_COVER,
    FIELD_YEAR,
    FIELD_FANART
  };


  CAlbumItem(QObject *parent = 0);

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};

#endif // ALBUMITEM_H
