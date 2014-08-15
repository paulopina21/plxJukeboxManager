#include "FanartManager.h"

CFanartManager* CFanartManager::INSTANCE = 0;
CFanartManager::CFanartManager(QObject* parent): QObject(parent)
{
  INSTANCE = this;
}
