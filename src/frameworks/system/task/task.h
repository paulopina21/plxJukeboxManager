#ifndef TASK_H
#define TASK_H

//#include <QObject>
#include <QThread>
#include "iTask.h"

class CTask : public iTask
{
  Q_OBJECT
  Q_INTERFACES(iTask)
public:
  explicit CTask(QObject *parent = 0);
  
signals:
  
public slots:
  
};

#endif // TASK_H
