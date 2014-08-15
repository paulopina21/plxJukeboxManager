#include "AppSettings.h"

CAppSettings* CAppSettings::INSTANCE = 0;

CAppSettings::CAppSettings(QObject *parent): QObject(parent)
{
  INSTANCE = this;

}
