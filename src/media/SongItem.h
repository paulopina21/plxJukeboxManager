#ifndef SONGITEM_H
#define SONGITEM_H

#include <plxFramework/models/ListItem.h>

class CSongItem: public ListItem
{
  Q_OBJECT
public:

  enum Roles
  {
    FIELD_ID = Qt::UserRole+1,
    FIELD_TRACK,
    FIELD_TITLE,
    FIELD_LENGTH,
    FIELD_GENRE,
    FIELD_LABEL,
    FIELD_YEAR,
    FIELD_FILEPATH
  };

  CSongItem(QObject* parent = 0);

  virtual int id() const { return data(FIELD_ID).toInt(); }
  virtual QHash<int, QByteArray> roleNames() const;
};

#endif // SONGITEM_H
