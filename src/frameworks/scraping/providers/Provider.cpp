#include "Provider.h"

#include <plxFramework/network/NetworkAccess.h>


QString CProvider::key()
{
  return m_strKey;
}

void CProvider::setKey(const QString& strKey)
{
  m_strKey = strKey;
}

QString CProvider::hostname()
{
  return m_strHostname;
}

void CProvider::setHostname(const QString& strHostname)
{
  m_strHostname = strHostname;
}

void CProvider::error(QNetworkReply *reply)
{
  reply->errorString();
}
