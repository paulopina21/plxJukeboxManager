#ifndef PFCFILE_H
#define PFCFILE_H

//#include <jni.h>

#include "../utils/Java.h"

class QString;

class CPFCFile
{
public:
  CPFCFile(const QString& strPackageName);
  ~CPFCFile();

  void setPackageName(const QString& strName);
  void setKey(const QString& strKey);
  long addFile(const QString& strFile);
  void make(const QString& strOutputFile);

private:
  jobject m_jPFCFile;
  jclass m_jPFCFileClass;

  CJava* m_java;
//  QString m_strClassName;
};

#endif // PFCFILE_H
