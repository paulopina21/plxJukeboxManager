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
  void showProgress(const QString& strTitle = "");
  void showSearchProgress(const QString& strMessage, const QString& strContent, const int& progress, const int& total);

public slots:
  void deleteObject();

private:
  QQuickItem* m_qmlRoot;
//  QQmlEngine *m_qmlEngine;
};

#endif // GUIDIALOG_H
