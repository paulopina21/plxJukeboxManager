#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QtQml>

#include <plxFramework/media/Artist.h>
#include <plxFramework/media/Song.h>

#include "../database/musicdatabase.h"
#include "../managers/CatalogsManager.h"

#include <plxFramework/utils/xml.h>

class CCollectionManager : public QObject
{
  Q_OBJECT
  static CCollectionManager* INSTANCE;
public:
  explicit CCollectionManager(QObject *parent = 0);
  static CCollectionManager* instance() { return INSTANCE; }

  Q_PROPERTY(QQmlListProperty<CArtist> artists READ artists CONSTANT)
  QQmlListProperty<CArtist> artists();

  Q_PROPERTY(QQmlListProperty<CArtist> fullArtists READ fullArtists CONSTANT)
  QQmlListProperty<CArtist> fullArtists();

  Q_INVOKABLE CArtist* getArtist(int index);
  Q_INVOKABLE QQmlListProperty<CArtist> getArtists();

//  Q_PROPERTY(QQmlListProperty<CAlbum> albums READ albums CONSTANT)
//  QQmlListProperty<CAlbum> albums();
//  Q_INVOKABLE QQmlListProperty<CAlbum> getAlbums(int idArtist);

//  Q_PROPERTY(QQmlListProperty<CSong> songs READ songs CONSTANT)
//  QQmlListProperty<CSong> songs() { return QQmlListProperty<CSong>(this, m_songs); }

  void addArtist(CArtist* artist);

  void addAlbum(CAlbum* album);

  Q_INVOKABLE bool isCollectionEmpty();

  Q_INVOKABLE int saveArtist(CArtist* artist);
  Q_INVOKABLE int saveAlbum(CAlbum* album);
  Q_INVOKABLE int saveSong(CSong* song);

  Q_INVOKABLE int buildAlbums();
  Q_INVOKABLE int listAlbums();

  Q_INVOKABLE void fullArtistsClean();
  Q_INVOKABLE void loadAllArtistView();
  Q_INVOKABLE void loadArtistView();


private:
  CMusicDatabase* m_musicDatabase;

  QList<CArtist*> m_artists;
  QList<CArtist*> m_fullArtists;

  QList<CAlbum*> m_albums;
  QList<CSong*> m_songs;

  QString m_strStorageDir;

  CMediaItem* albumBuild(int idAlbum);
  QList<CSong*> songsBuild(int idAlbum);

  bool xmlBuild(CMediaItem* mediaItem);
  bool qrcBuild(CMediaItem* mediaItem);
  bool rccBuild(CMediaItem* mediaItem);

  QString m_albumDir;
  QString m_plxFile;
  QString m_qrcFile;
  QString m_coverFile;
  QString m_fanartFile;
  QString m_rccFile;

signals:
  void artistsChanged();
  void albumsChanged();
  void viewChanged();
  void updateProgress();
  void updateFinished();

public slots:
};

#endif // MAINMODEL_H
