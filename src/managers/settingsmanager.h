#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
//#include "../frameworks/application/Settings.h"

#include "../frameworks/application/configgroup.h"

class CSettingsManager : public QObject
{
  Q_OBJECT
public:
  explicit CSettingsManager(QObject *parent = 0);
  static CSettingsManager* instance() { return INSTANCE; }

signals:
  
public slots:
  void load();
  ListModel* settings(){ return m_items; }

private:
  static CSettingsManager* INSTANCE;
  void populateChildren(const QStringList& keys, CConfigGroup* parentGroup);

  CConfigGroup::ConfigType typeOf(const QVariant& value);

  ListModel* m_items;
  QSettings* m_settings;
};

#endif // SETTINGSMANAGER_H
