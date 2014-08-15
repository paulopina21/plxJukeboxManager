#ifndef QRCMANAGER_H
#define QRCMANAGER_H

#include <QObject>

class TiXmlDocument;
class TiXmlNode;
class RCCResourceLibrary;

class CQRCFileBuilder : public QObject
{
  Q_OBJECT
public:
  explicit CQRCFileBuilder(const QString& strQRCName, QObject *parent = 0);

signals:

public slots:
  bool addFile(const QString& strFilePath, const QString& strAlias);
  bool save(const QString& strFilePath);

private:
  TiXmlDocument* m_xmlDoc;
  TiXmlNode* m_xmlRootNode;
  TiXmlNode* m_xmlResourcesNode;
  RCCResourceLibrary* m_RCCFile;
  QString m_strPrefix;

};

#endif // QRCMANAGER_H
