#ifndef ITASK_H
#define ITASK_H

#include <QThread>

class iTask: public QThread
{
  Q_OBJECT
public:
  iTask(QObject* parent = 0): QThread(parent) {}
  virtual ~iTask() {}
  virtual void setExit(bool bExit) = 0;

signals:
  //! loading() Should be used when loading data for working or when creating the task
  void loading();

  //! loaded() Should be used after loaded data for work or after created the task
  void loaded();

  //! working() Use it when stated the work
  void working();

  //! done() Use it when the work is done
  void done();
};

#define iTask_iid "com.plexusdynamics.frameworks.system.task.iTask"
Q_DECLARE_INTERFACE(iTask, iTask_iid)


#endif // ITASK_H
