#include "PFCFile.h"

#include <QString>

#include <plxFramework/utils/log.h>

#define STR_CLASS_NAME "com/plexusdynamics/system/PFCFile"

//#define CLASS_NAME com/plexusdynamics/system/PFCFile

CPFCFile::CPFCFile(const QString& strPackageName)
{
  m_java = CJava::instance();

  m_jPFCFileClass = m_java->jni()->FindClass(STR_CLASS_NAME);
  if (!m_jPFCFileClass)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't load the java file", "JVM");
    return;
  }

  m_jPFCFile = m_java->jni()->AllocObject(m_jPFCFileClass);
  if (!m_jPFCFile)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't create the java object", "JVM");
    return;
  }

}

CPFCFile::~CPFCFile()
{
  m_java->jni()->DeleteLocalRef(m_jPFCFileClass);
  m_java->jni()->DeleteLocalRef(m_jPFCFile);
}

void CPFCFile::setPackageName(const QString& strName)
{
  if (!m_jPFCFileClass) return;
  jmethodID jMID = m_java->jni()->GetMethodID(m_jPFCFileClass, "setPackageName", "(Ljava/lang/String;)V");

  if (jMID == 0)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't load the method.", "JVM");
    return;
  }

  jstring property = m_java->jni()->NewStringUTF(strName.toStdString().c_str());
  m_java->jni()->CallVoidMethod(m_jPFCFile, jMID, property);
}

void CPFCFile::setKey(const QString& strKey)
{
  if (!m_jPFCFileClass) return;
  jmethodID jMID = m_java->jni()->GetMethodID(m_jPFCFileClass, "setKey", "(Ljava/lang/String;)V");

  if (jMID == 0)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't load the method.", "JVM");
    return;
  }

  jstring property = m_java->jni()->NewStringUTF(strKey.toStdString().c_str());
  m_java->jni()->CallVoidMethod(m_jPFCFile, jMID, property);
}

long CPFCFile::addFile(const QString& strFile)
{
  if (!m_jPFCFileClass) return 0;
  jmethodID jMID = m_java->jni()->GetMethodID(m_jPFCFileClass, "addFile", "(Ljava/lang/String;)J");

  if (jMID == 0)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't load the method.", "JVM");
    return 0;
  }

  jstring property = m_java->jni()->NewStringUTF(strFile.toStdString().c_str());
  jlong ret = m_java->jni()->CallLongMethod(m_jPFCFile, jMID, property);
  m_java->jni()->ExceptionDescribe();
  return (long) ret;
}

void CPFCFile::make(const QString& strOutputFile)
{
  if (!m_jPFCFileClass) return;
  jmethodID jMID = m_java->jni()->GetMethodID(m_jPFCFileClass, "make", "(Ljava/lang/String;)V");

  if (jMID == 0)
  {
    m_java->jni()->ExceptionDescribe();
    CLog::log(LOG_FATAL, "PFC MANAGER", "Couldn't load the method.", "JVM");
    return;
  }

  jstring property = m_java->jni()->NewStringUTF(strOutputFile.toStdString().c_str());
  m_java->jni()->CallVoidMethod(m_jPFCFile, jMID, property);
  m_java->jni()->ExceptionDescribe();
}
