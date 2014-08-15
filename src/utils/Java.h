#ifndef JAVA_H
#define JAVA_H

#include <jni.h>

/**
Signature   Java Type
Z           boolean
B           byte
C           char
S           short
I           int
J           long
F           float
D           double
L           fully-qualified-class ;	 fully-qualified-class
[ type      type[]

( arg-types ) ret-type	 method type
**/


class CJava
{
  static CJava* INSTANCE;
public:
  CJava();
  ~CJava();
  static CJava* instance();

  JavaVM* jvm();
  JNIEnv* jni();

private:
  JavaVM* m_jVM;
  JNIEnv* m_jEnv;
  JavaVMInitArgs m_jArgs;

  const char *getPropertyValue(const char *name);
};

#endif // JAVA_H
