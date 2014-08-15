#ifndef GUIVIEW_H
#define GUIVIEW_H

#include <QObject>
#include <QMap>

class ListModel;

class CGUIView : public QObject
{
  Q_OBJECT
public:
  explicit CGUIView(QObject *parent = 0): QObject(parent) {}

signals:
  void loading();
  void loaded();
  void finished();
  
public slots:
  virtual void load() = 0;
  virtual void unload() = 0;

protected:
  QMap<int, ListModel*> m_lists;
};

#endif // GUIVIEW_H
