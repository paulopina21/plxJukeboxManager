#include "guidialog.h"
#include <QQmlComponent>

#include "../core/Application.h"

CGUIDialog::CGUIDialog(QQuickItem *root, QObject *parent): QObject(parent)
{
  m_qmlRoot = root;
//  m_qmlObject = NULL;
//  m_qmlEngine = engine;
}

void CGUIDialog::show(const QString& strMessage, const QString &strTitle)
{
//  QObject* container_main = CApplication::instance()->rootObject();
  QMetaObject::invokeMethod(m_qmlRoot, "showDialog", Q_ARG(QVariant, strMessage), Q_ARG(QVariant, strTitle));

//  QQmlEngine* engine = CApplication::instance()->engine();
//  QQmlComponent component(engine, QUrl::fromLocalFile("qrc:/qml/components/Dialog.qml"), this);
//  QObject* loader_main = CApplication::instance()->rootObject()->findChild<QObject*>("loader_main");

//  QObject *object = component.create();
//  QMetaObject::invokeMethod(object, "setBlurTarget", Q_ARG(QObject*, loader_main));
}

void CGUIDialog::showProgress(const QString &strTitle)
{
  QMetaObject::invokeMethod(m_qmlRoot, "showProgressDialog", Q_ARG(QVariant, strTitle));
}

void CGUIDialog::showSearchProgress(const QString& strMessage, const QString& strContent, const int& progress, const int& total){
  QMetaObject::invokeMethod(m_qmlRoot, "showSearchProgress", Q_ARG(QVariant, strMessage), Q_ARG(QVariant, strContent), Q_ARG(QVariant, progress), Q_ARG(QVariant, total));
}

void CGUIDialog::deleteObject()
{
//  delete m_qmlObject;
}
