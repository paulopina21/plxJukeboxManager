#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include "../frameworks/application/ApplicationSettings.h"

class CAppSettings : public QObject
{
  Q_OBJECT
  static CAppSettings* INSTANCE;
public:
  explicit CAppSettings(QObject *parent = 0);
  CAppSettings* instance() { return INSTANCE; }

  void loadSettings();
  void saveSettings();

  Q_INVOKABLE void setValue(const QString& strGroup, const QString& strName, const QVariant& value);
  Q_INVOKABLE QVariant getValue(const QString& strGroup, const QString& strName, const QVariant &defaultValue = "");

private:
  QMap<QString, QMap<QString, QVariant> > m_values;
signals:
  
public slots:
  
};

#endif // APPSETTINGS_H
