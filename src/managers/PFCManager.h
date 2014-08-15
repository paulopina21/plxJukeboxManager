#ifndef PFCMANAGER_H
#define PFCMANAGER_H

#include <plxFramework/concurrent/Worker.h>

class CMusicDatabase;

class CPFCManager: public CWorker
{
  Q_OBJECT
public:
  CPFCManager(QObject* parent = 0);
  virtual int id() const {}
  virtual QHash<int, QByteArray> roleNames() const {}

public slots:
  virtual void run() {}

  void buildCatalog();

  void makePFC(int idAlbum);
private:
  CMusicDatabase* m_musicDatabase;

  void buildPFC(const QStringList& files, const QString& strFileName);
  void buildQRC(const QStringList& files, const QString& strFileName);
};

#endif // PFCMANAGER_H
