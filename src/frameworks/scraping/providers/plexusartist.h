#ifndef PLEXUSARTIST_H
#define PLEXUSARTIST_H

#include <plxFramework/scraping/Provider.h>

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>

enum Params
{
  ID,
  ARTIST,
  FANART,
  FANART_THUMB,
  API_KEY,
  PROVIDER
};

class CPlexusArtist  : public iProvider
{
Q_OBJECT

public:
    CPlexusArtist(QObject* parent = 0);

    void set_search(const QString& artist);
    void work(int id);

private slots:
    void requestInfo(int id);
    void parseInfo(QByteArray);
    void receivedInfo(QByteArray);
    void stopSearch();
    const QString GetLegalUrlName(const QString& strFileName);

public slots:
    QString artist() { return m_params[ARTIST].toString(); }
    QString fanartName() { return m_params[FANART].toString(); }
    QString apiKey() { return m_APIKey; }

signals:
    void notFound(int);

private:
    QString      m_APIKey;

    QMap<QObject*, int> _listRequests;
};
#endif // PLEXUSARTIST_H
