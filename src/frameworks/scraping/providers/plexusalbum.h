#ifndef PLEXUSALBUM_H
#define PLEXUSALBUM_H

#include <plxFramework/scraping/Provider.h>

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>

enum Params
{
  ID,
  ARTIST,
  TITLE,
  COVER,
  COVER_THUMB,
  API_KEY,
  PROVIDER
};

class CPlexusAlbum  : public iProvider
{
Q_OBJECT

public:
    CPlexusAlbum(QObject* parent = 0);

    void set_search(const QString& artist, const QString& title);
    void work(int id);

private slots:
    void requestInfo(int id);
    void parseInfo(QByteArray);
    void receivedInfo(QByteArray);
    void stopSearch();
    const QString GetLegalUrlName(const QString& strFileName);

public slots:
    QString artist() { return m_params[ARTIST].toString(); }
    QString title() { return m_params[TITLE].toString(); }
    QString coverName() { return m_params[COVER].toString(); }
    QString apiKey() { return m_APIKey; }

signals:
    void notFound(int);

private:
    QString      m_APIKey;

    QMap<QObject*, int> _listRequests;
};

#endif // PLEXUSALBUM_H
