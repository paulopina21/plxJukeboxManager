#include <QtQml>
//#include <plxFramework/security/cryptography/cryptoprovider.h>
#include "../gui/guidialog.h"
#include "../core/Application.h"

CJukeboxManager* CJukeboxManager::INSTANCE = 0;
//MountedDevices* CJukeboxManager::m_initialDevices = 0;//MountProvider::getDrives(LIST_REMOVABLE_ONLY);

CJukeboxManager::CJukeboxManager(QObject *parent): QObject(parent)
{
  INSTANCE = this;
  qmlRegisterType<CJukeboxManager>("jukebox", 1, 0, "JukeboxManager");
}
