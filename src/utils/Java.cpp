#include "Java.h"
#include <plxFramework/utils/log.h>

CJava* CJava::INSTANCE = 0;
CJava::CJava()
{
  if (INSTANCE) return;
  INSTANCE = this;

  jint ret;

  m_jArgs.version = JNI_VERSION_1_6;
  m_jArgs.nOptions = 0;
  m_jArgs.ignoreUnrecognized = JNI_FALSE;

  ret = JNI_CreateJavaVM(&m_jVM, (void **) &m_jEnv, &m_jArgs);
  CLog::log(LOG_DEBUG, "PFC MANAGER", "Version: " + QString(getPropertyValue("java.version")), "JVM");
  CLog::log(LOG_DEBUG, "PFC MANAGER", "Vendor: " + QString(getPropertyValue("java.vendor")), "JVM");
}

CJava::~CJava()
{
  m_jVM->DestroyJavaVM();
}

CJava* CJava::instance()
{
  if (!INSTANCE) new CJava();
  return INSTANCE;
}

JavaVM*CJava::jvm()
{
  return m_jVM;
}

JNIEnv*CJava::jni()
{
  return m_jEnv;
}

const char*CJava::getPropertyValue(const char* name)
{
  jclass systemClass = this->jni()->FindClass("java/lang/System");
  jmethodID getPropertyMethod = this->jni()->GetStaticMethodID(systemClass, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
  jstring propertyName = this->jni()->NewStringUTF(name);
  jstring value = (jstring) this->jni()->CallStaticObjectMethod(systemClass, getPropertyMethod, propertyName);

  return this->jni()->GetStringUTFChars(value, NULL);
}
