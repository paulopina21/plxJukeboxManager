#include "PFCManager.h"

#include <QFileInfo>

#include <plxFramework/utils/log.h>
#include <plxFramework/utils/xml.h>
#include <plxFramework/application/Settings.h>

#include "../media/PFCFile.h"
#include "../database/musicdatabase.h"
#include "../core/Application.h"
#include "../managers/CatalogsManager.h"
#include "../managers/QRCFileBuilder.h"

CPFCManager::CPFCManager(QObject* parent)
{
  m_musicDatabase = CMusicDatabase::instance();
  m_musicDatabase->open();
//  CPFCFile* pfcFile = new CPFCFile("");
//  pfcFile->setPackageName("TESTING");
////  pfcFile->setKey("");
//  pfcFile->addFile("/Volumes/LAUREON/test.mp3");
//  pfcFile->addFile("/Volumes/LAUREON/test.flac");
  //  pfcFile->make("/Volumes/LAUREON/test.jobb");
}

void CPFCManager::buildCatalog()
{
//  QString strSQL = m_musicDatabase->prepareSQL("SELECT idAlbum, strArtist FROM albumview ORDER BY strArtist");
  QSqlQuery query("SELECT idAlbum, strAlbum FROM albumview ORDER BY strArtist, strAlbum", *m_musicDatabase->db());
  while (query.next())
  {
    CLog::log(LOG_INFO, "PFC MANAGER", "Generating PFC for: " + query.value(1).toString());
    makePFC(query.value(0).toInt());
  }
}

void CPFCManager::makePFC(int idAlbum)
{
  QString strSQL = m_musicDatabase->prepareSQL("SELECT idAlbum, idArtist, strArtist, strAlbum, iYear, strGenre, GTIN, strThumb, strFanart FROM albumview WHERE idAlbum = %i", idAlbum);
  QSqlQuery query(strSQL, *m_musicDatabase->db());
  if (!query.first()) return;

  TiXmlDocument xmlDoc;
  TiXmlDeclaration decl("1.0", "UTF-8", "yes");
  xmlDoc.InsertEndChild(decl);
  TiXmlNode *pMain = &xmlDoc;

  QStringList filesToAdd;

  QFileInfo fileFanart(CSettings::folder(CSettings::FOLDER_FANARTS) + query.value(8).toString());
  QFileInfo fileCover(CSettings::folder(CSettings::FOLDER_ALBUMS) + query.value(7).toString());

  if (fileFanart.exists())
    filesToAdd << fileFanart.absoluteFilePath();

  if (fileCover.exists())
    filesToAdd << fileCover.absoluteFilePath();

  TiXmlElement artistElement("artist");
  TiXmlNode* artistNode = pMain->InsertEndChild(artistElement);
  XMLUtils::SetString(artistNode,"name", query.value(2).toString());
  XMLUtils::SetString(artistNode,"fanart", fileFanart.fileName());

  TiXmlElement albumElement("album");
  TiXmlNode* albumNode = pMain->InsertEndChild(albumElement);
  XMLUtils::SetString(albumNode,"artist", query.value(2).toString());
  XMLUtils::SetString(albumNode,"title", query.value(3).toString());
  XMLUtils::SetString(albumNode,"year", query.value(4).toString());
  XMLUtils::SetString(albumNode,"genre", query.value(5).toString());
  XMLUtils::SetString(albumNode,"GTIN", query.value(6).toString());
  XMLUtils::SetString(albumNode,"cover", fileCover.fileName());

  QString strSQLSongs = m_musicDatabase->prepareSQL("SELECT strArtist, iTrack, strTitle, strDuration, iYear, strGenre, ISRC, isVideo, strLabel, strFilename FROM songview WHERE idAlbum = %i ORDER BY iTrack", idAlbum);
  QSqlQuery querySongs(strSQLSongs, *m_musicDatabase->db());

  while(querySongs.next())
  {
    QFileInfo fileSong(querySongs.value(9).toString());

//    if (!fileSong.exists()) continue;

//    filesToAdd << fileSong.absoluteFilePath();

    QString strTrack = QString("%1").arg(querySongs.value("iTrack").toInt(), 2, 'd', 0, '0');


    TiXmlElement songElement("song");
    TiXmlNode* songNode = albumNode->InsertEndChild(songElement);
    XMLUtils::SetString(songNode,"artist", querySongs.value("strArtist").toString());
    XMLUtils::SetString(songNode,"track", strTrack);
    XMLUtils::SetString(songNode,"title", querySongs.value("strTitle").toString());
    XMLUtils::SetString(songNode,"length", querySongs.value("strDuration").toString());
    XMLUtils::SetString(songNode,"year", querySongs.value("iYear").toString());
    XMLUtils::SetString(songNode,"genre", querySongs.value("strGenre").toString());
    XMLUtils::SetString(songNode,"isrc", querySongs.value("ISRC").toString());
    XMLUtils::SetString(songNode,"video", querySongs.value("isVideo").toString());
    XMLUtils::SetString(songNode,"label", querySongs.value("strLabel").toString());
    XMLUtils::SetString(songNode,"filename", fileSong.fileName());
  }

  QString strXMLFile( CSettings::folder(CSettings::FOLDER_TEMP) + "/playlist.plx" );

  if ( xmlDoc.SaveFile(strXMLFile.toStdString().c_str()) ) // TODO: LAUREON: You know what to do...
    filesToAdd << strXMLFile;

  QString strOutputPath(  CCatalogsManager::instance()->currentCatalog()->data(CCatalog::FIELD_OUTPUTPATH).toString() );

  CLog::log(LOG_DEBUG, "PFC MANAGER", "Output path: " + strOutputPath);

  buildQRC(filesToAdd, query.value(2).toString() + " - " + query.value(3).toString());
}

void CPFCManager::buildPFC(const QStringList& files, const QString& strFileName)
{
//  CLog::log(LOG_DEBUG, "PFC MANAGER", "Building for: " + strFileName, "PFC-GEN");
//  CPFCFile* pfcFile = new CPFCFile("");
//  pfcFile->setPackageName("com.plexusdynamics.media.jukebox.pro");

//  for (QStringList::const_iterator strFile = files.constBegin(); strFile != files.constEnd(); ++strFile)
//  {
//    pfcFile->addFile(*strFile);
//  }

//  pfcFile->addFile("/Volumes/LAUREON/test.flac");

//  pfcFile->make("/Volumes/LAUREON/test/" + strFileName + ".pfc");
//  delete pfcFile;
}

void CPFCManager::buildQRC(const QStringList& files, const QString& strFileName)
{
  CLog::log(LOG_DEBUG, "PFC MANAGER", "Building QRC for: " + strFileName, "PFC-GEN");
  CQRCFileBuilder* pfcFile = new CQRCFileBuilder(strFileName);
//  pfcFile->setPackageName("com.plexusdynamics.media.jukebox.pro");

  for (QStringList::const_iterator strFile = files.constBegin(); strFile != files.constEnd(); ++strFile)
  {
    pfcFile->addFile(*strFile, "");
  }

//  pfcFile->addFile("/Volumes/JUKEBOX/test.mp3", "test.mp3");

  QString strOutput = CCatalogsManager::instance()->currentCatalog()->data(CCatalog::FIELD_OUTPUTPATH).toString() + "/";
  QString strOutFilePath = strOutput + strFileName + ".rcc"; // Produto final

  QString strQRCFile = CSettings::folder(CSettings::FOLDER_TEMP) + "/temp.qrc"; // Blueprint

  if ( pfcFile->save( strQRCFile ) )
  {
    // rcc -no-compress	-binary -o strOutFilePath strQRCFile
    // Chamar rcc com os argumentos.
    // Saber quando o rcc terminou de gerar

    QStringList args;

    args << strQRCFile;
    args << "-binary";
    args << "-o" << strOutFilePath;

    QProcess rccProc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    QString qtDirPath = env.value("QTDIR");
//    bool isLoaded = false;
//    rccProc.setWorkingDirectory(qtDirPath + "bin/");
    rccProc.execute("rcc", args);

//    if (rccProc.execute("rcc", args)) // rcc executed successfully
//    {
//        isLoaded = QResource::registerResource(rccPath);
//        if (isLoaded)
//            ui->label->setPixmap(QPixmap(":/test.jpg"));
//    }
  }

  delete pfcFile;
}
