#ifndef GUIDIALOG_H
#define GUIDIALOG_H

//#include <QQmlComponent>
#include <QQuickItem>

class CGUIDialog: public QObject
{
  Q_OBJECT
public:
  CGUIDialog(QQuickItem *root, QObject * parent = 0);
  void show(const QString& strMessage, const QString& strTitle = "");

public slots:
  void deleteObject();

private:
  QQuickItem* m_qmlRoot;
//  QQmlEngine *m_qmlEngine;
};

#endif // GUIDIALOG_H
