#include "QRCFileBuilder.h"

#include <QFileInfo>

#include <plxFramework/utils/log.h>
#include <plxFramework/utils/xml.h>
#include <plxFramework/filesystem/Rcc.h>



/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//#include <qdebug.h>
//#include <qdir.h>
//#include <qfile.h>
//#include <qfileinfo.h>
//#include <qtextstream.h>
//#include <qatomic.h>
//#include <qglobal.h>
//#include <qcoreapplication.h>
//#include <qcommandlineoption.h>
//#include <qcommandlineparser.h>


//QT_BEGIN_NAMESPACE

//void dumpRecursive(const QDir &dir, QTextStream &out)
//{
//    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot
//                                              | QDir::NoSymLinks);
//    foreach (const QFileInfo &entry, entries) {
//        if (entry.isDir()) {
//            dumpRecursive(entry.filePath(), out);
//        } else {
//            out << QLatin1String("<file>")
//                << entry.filePath()
//                << QLatin1String("</file>\n");
//        }
//    }
//}

//int createProject(const QString &outFileName)
//{
//    QDir currentDir = QDir::current();
//    QString currentDirName = currentDir.dirName();
//    if (currentDirName.isEmpty())
//        currentDirName = QLatin1String("root");

//    QFile file;
//    bool isOk = false;
//    if (outFileName.isEmpty()) {
//        isOk = file.open(stdout, QFile::WriteOnly | QFile::Text);
//    } else {
//        file.setFileName(outFileName);
//        isOk = file.open(QFile::WriteOnly | QFile::Text);
//    }
//    if (!isOk) {
//        fprintf(stderr, "Unable to open %s: %s\n",
//                outFileName.isEmpty() ? qPrintable(outFileName) : "standard output",
//                qPrintable(file.errorString()));
//        return 1;
//    }

//    QTextStream out(&file);
//    out << QLatin1String("<!DOCTYPE RCC><RCC version=\"1.0\">\n"
//                         "<qresource>\n");

//    // use "." as dir to get relative file pathes
//    dumpRecursive(QDir(QLatin1String(".")), out);

//    out << QLatin1String("</qresource>\n"
//                         "</RCC>\n");

//    return 0;
//}

//int runRcc(int argc, char *argv[])
//{
//    QCoreApplication app(argc, argv);
//    QCoreApplication::setApplicationVersion(QString::fromLatin1(QT_VERSION_STR));

//    // Note that rcc isn't translated.
//    // If you use this code as an example for a translated app, make sure to translate the strings.
//    QCommandLineParser parser;
//    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
//    parser.setApplicationDescription(QStringLiteral("Qt Resource Compiler version %1").arg(QString::fromLatin1(QT_VERSION_STR)));
//    parser.addHelpOption();
//    parser.addVersionOption();

//    QCommandLineOption outputOption(QStringList() << QStringLiteral("o") << QStringLiteral("output"));
//    outputOption.setDescription(QStringLiteral("Write output to <file> rather than stdout."));
//    outputOption.setValueName(QStringLiteral("file"));
//    parser.addOption(outputOption);

//    QCommandLineOption nameOption(QStringLiteral("name"), QStringLiteral("Create an external initialization function with <name>."), QStringLiteral("name"));
//    parser.addOption(nameOption);

//    QCommandLineOption rootOption(QStringLiteral("root"), QStringLiteral("Prefix resource access path with root path."), QStringLiteral("path"));
//    parser.addOption(rootOption);

//    QCommandLineOption compressOption(QStringLiteral("compress"), QStringLiteral("Compress input files by <level>."), QStringLiteral("level"));
//    parser.addOption(compressOption);

//    QCommandLineOption nocompressOption(QStringLiteral("no-compress"), QStringLiteral("Disable all compression."));
//    parser.addOption(nocompressOption);

//    QCommandLineOption thresholdOption(QStringLiteral("threshold"), QStringLiteral("Threshold to consider compressing files."), QStringLiteral("level"));
//    parser.addOption(thresholdOption);

//    QCommandLineOption binaryOption(QStringLiteral("binary"), QStringLiteral("Output a binary file for use as a dynamic resource."));
//    parser.addOption(binaryOption);

//    QCommandLineOption namespaceOption(QStringLiteral("namespace"), QStringLiteral("Turn off namespace macros."));
//    parser.addOption(namespaceOption);

//    QCommandLineOption verboseOption(QStringLiteral("verbose"), QStringLiteral("Enable verbose mode."));
//    parser.addOption(verboseOption);

//    QCommandLineOption listOption(QStringLiteral("list"), QStringLiteral("Only list .qrc file entries, do not generate code."));
//    parser.addOption(listOption);

//    QCommandLineOption projectOption(QStringLiteral("project"), QStringLiteral("Output a resource file containing all files from the current directory."));
//    parser.addOption(projectOption);

//    parser.addPositionalArgument(QStringLiteral("inputs"), QStringLiteral("Input files (*.qrc)."));


//    //parse options
//    parser.process(app);

//    QString errorMsg;
//    RCCResourceLibrary library;
//    QString outFilename = parser.value(outputOption);
////    if (parser.isSet(nameOption))
////        library.setInitName(parser.value(nameOption));

////    if (parser.isSet(rootOption))
////    {
////        library.setResourceRoot(QDir::cleanPath(parser.value(rootOption)));
////        if (library.resourceRoot().isEmpty()
////                || library.resourceRoot().at(0) != QLatin1Char('/'))
////            errorMsg = QLatin1String("Root must start with a /");
////    }

//    if (parser.isSet(compressOption))
//        library.setCompressLevel(parser.value(compressOption).toInt());
//    if (parser.isSet(nocompressOption))
//        library.setCompressLevel(-2);
//    if (parser.isSet(thresholdOption))
//        library.setCompressThreshold(parser.value(thresholdOption).toInt());
////    if (parser.isSet(binaryOption))
////        library.setFormat(RCCResourceLibrary::Binary);
////    if (parser.isSet(namespaceOption))
////        library.setUseNameSpace(!library.useNameSpace());
////    if (parser.isSet(verboseOption))
////        library.setVerbose(true);

//    const bool list = parser.isSet(listOption);
//    const QStringList filenamesIn = parser.positionalArguments();

//    if (!errorMsg.isEmpty()) {
//        fprintf(stderr, "%s: %s\n", argv[0], qPrintable(errorMsg));
//        parser.showHelp(1);
//        return 1;
//    }

//}

//Q_CORE_EXPORT extern QBasicAtomicInt qt_qhash_seed; // from qhash.cpp

//QT_END_NAMESPACE

//=============================================================================
CQRCFileBuilder::CQRCFileBuilder(const QString& strQRCName, QObject *parent) :
  QObject(parent)
{
  m_xmlDoc      = new TiXmlDocument();
  m_xmlRootNode = m_xmlDoc;
//  TiXmlDeclaration decl("1.0", "UTF-8", "yes");
//  m_xmlDoc->InsertEndChild(decl);

  QString strName = "/" + strQRCName + "/";
  TiXmlElement rccElement("RCC");
  m_xmlResourcesNode = m_xmlRootNode->InsertEndChild(rccElement);

  TiXmlElement resourceElement("qresource");
  resourceElement.SetAttribute("prefix",strName.toStdString().c_str());
  m_xmlResourcesNode = m_xmlResourcesNode->InsertEndChild(resourceElement);



  m_RCCFile = new RCCResourceLibrary;
//  m_RCCFile->reset();

#ifdef QT_DEBUG
  m_RCCFile->setVerbose(true);
#endif

  m_RCCFile->setFormat(RCCResourceLibrary::Binary);

  m_strPrefix = strQRCName;

//  m_RCCFile->setResourceRoot(strName);
//  m_RCCFile->setCompressLevel(0);
}

//<qresource prefix="/myresources">
//  <file alias="cut-img.png">images/cut.png</file>
//</qresource>

bool CQRCFileBuilder::addFile(const QString& strFilePath, const QString& strAlias)
{
//  QFileInfo file(strFilePath);
//  if (!file.exists()) return false;

//  QString strName = file.fileName();
//  return m_RCCFile->addFile(strFilePath, strName, m_strPrefix);

  QFileInfo file(strFilePath);
  if (!file.exists()) return false;

  QString strName = file.fileName();

  TiXmlText* text = new TiXmlText(file.absoluteFilePath().toStdString().c_str());

  TiXmlElement resourceElement("file");
  resourceElement.SetAttribute("alias", strName.toStdString().c_str());
  resourceElement.InsertEndChild(*text);

  m_xmlResourcesNode->InsertEndChild(resourceElement);

  return true;
}

bool CQRCFileBuilder::save(const QString& strFilePath)
{
//  QFile out;
//  QIODevice::OpenMode mode = QIODevice::WriteOnly;

//  if (m_RCCFile->format() == RCCResourceLibrary::C_Code)
//  {
//    mode |= QIODevice::Text;
//    out.setFileName(strFilePath + ".c");
//  }
//  else
//    out.setFileName(strFilePath);

//  if (!out.open(mode))
//  {
//    const QString msg = QString::fromUtf8("Unable to open %1 for writing: %2\n").arg(strFilePath).arg(out.errorString());
//    CLog::log(LOG_ERROR, "QRC_BUILDER", msg);
//    return false;
//  }

//  return m_RCCFile->save(out);
  return m_xmlDoc->SaveFile(strFilePath.toStdString().c_str());
}
