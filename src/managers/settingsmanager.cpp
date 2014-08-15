#include "settingsmanager.h"
#include <plxFramework/models/ListModel.h>
#include <QDebug>
#include <QtQml>

CSettingsManager* CSettingsManager::INSTANCE = 0;

CSettingsManager::CSettingsManager(QObject *parent): QObject(parent)
{
  INSTANCE = this;
  m_items = new ListModel(new CConfigGroup(this), this);
  m_settings = new QSettings();
  qDebug() << "ApplicationSettings -> Settings file:" << m_settings->fileName();

  qmlRegisterType<CSettingsManager>("core",1,0,"SettingsManager");

  load();
//  if (!CSettings::instance())
//    new CSettings(this);
}

void CSettingsManager::load()
{
  QStringList groups = m_settings->childGroups();
  foreach (QString group, groups)
  {
    if (group == "com") continue;

    m_settings->beginGroup(group);
    CConfigGroup* configGroup = new CConfigGroup(this);
    configGroup->setType(CConfigGroup::GROUP);
    configGroup->setGroup(group);
    configGroup->setName(group);

// KEYS -----------------------------------------------------------------------
    QStringList keys = m_settings->childKeys();
    populateChildren(keys,configGroup);
//-----------------------------------------------------------------------------

// GROUPS ---------------------------------------------------------------------
    QStringList childArrays = m_settings->childGroups();
    if (childArrays.size() > 0)
    {
      foreach (QString array, childArrays)
      {
        CConfigGroup* childGroup = new CConfigGroup(configGroup);
        childGroup->setGroup(group);
        childGroup->setName(array);
        childGroup->setType(CConfigGroup::GROUP);

        int count = m_settings->beginReadArray(array);
        for (int i=0 ; i<count ; ++i)
        {
          m_settings->setArrayIndex(i);
          QStringList childKeys = m_settings->childKeys();
          populateChildren(childKeys,childGroup);
        }
        configGroup->addChild(childGroup);
        m_settings->endArray();
      }

    }


    //-----------------------------------------------------------------------------
    m_settings->endGroup();
    settings()->appendRow(configGroup);
  }
}

void CSettingsManager::populateChildren(const QStringList &keys, CConfigGroup* parentGroup)
{
  if (keys.size() > 0)
  {
    foreach (QString key, keys)
    {
      CConfigGroup* configKey = new CConfigGroup(parentGroup);
      configKey->setGroup(parentGroup->name());
      configKey->setName(key);

      QVariant value = m_settings->value(key);
      configKey->setType(typeOf(value));
      configKey->setValue(value);
      parentGroup->addChild(configKey);
    }
  }
}

CConfigGroup::ConfigType CSettingsManager::typeOf(const QVariant &value)
{
  switch (value.type()) {
    case QMetaType::Bool:
      return CConfigGroup::BOOL;
    break;
    case QMetaType::Int:
      return CConfigGroup::INT;
    break;
    case QMetaType::QString:
      return CConfigGroup::TEXT;
    break;
    case QMetaType::QStringList:
      return CConfigGroup::ARRAY;
    break;
    default:
    break;
  }
}
