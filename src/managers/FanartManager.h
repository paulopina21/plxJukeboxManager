#ifndef FANARTMANAGER_H
#define FANARTMANAGER_H

#include <QObject>

class CFanartManager: public QObject
{
  Q_OBJECT
  static CFanartManager* INSTANCE;
public:
  CFanartManager(QObject* parent = 0);
  static CFanartManager* instance() { if(!INSTANCE) new CFanartManager(); return INSTANCE; }






};

#endif // FANARTMANAGER_H
