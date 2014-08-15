#ifndef JUKEBOXMANAGER_H
#define JUKEBOXMANAGER_H

#include <QObject>
#include <plxFramework/filesystem/mountprovider.h>

//ff1cf3dc642f76e2

class CJukeboxManager : public QObject
{
  Q_OBJECT
  static CJukeboxManager* INSTANCE;
public:
  explicit CJukeboxManager(QObject *parent = 0);
  static CJukeboxManager* instance() { return INSTANCE; }
};

#endif // JUKEBOXMANAGER_H
