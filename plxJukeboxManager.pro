# Add more folders to ship with the application, here
folder_01.source = qml
DEPLOYMENTFOLDERS = folder_01

DEFINES += CUSTOMER_MEGATRONI

QT += sql multimediawidgets widgets #qml quick sql opengl multimedia

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

macx {
  INCLUDEPATH += /usr/local/include
  INCLUDEPATH += /usr/include

#  INCLUDEPATH += /System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers
#  LIBS += -L/System/Library/Frameworks/JavaVM.framework/Libraries
#  LIBS += -framework JavaVM

  LIBS += -L/usr/local/lib
  LIBS += -L/usr/lib

  CONFIG(release, debug|release) {
    message(Release plxFramework Selected!)
    LIBS += -L$$_PRO_FILE_PWD_/../../build/plxFramework/Desktop_Qt_5_2_0_clang_64bit-Release
  }
  CONFIG(debug, debug|release) {
    message(Debug plxFramework Selected!)
    LIBS += -L$$_PRO_FILE_PWD_/../../build/plxFramework/Desktop_Qt_5_2_0_clang_64bit-Debug
  }

  INCLUDEPATH += ../

  LIBS += -ltag #-lcryptopp

  OUTPUT = $$OUT_PWD/$${TARGET}.app/Contents/MacOS/
}

LIBS += -lplxFW

CP_COMMAND = cp -r $${PWD}/java/* $$OUTPUT
message(When building I will execute: $$CP_COMMAND)
system($$CP_COMMAND)

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    src/core/Application.cpp \
    src/database/musicdatabase.cpp \
    src/database/databasebuilder.cpp \
#    src/core/AppSettings.cpp \
    src/core/ThreadManager.cpp \
    src/core/CollectionManager.cpp \
    src/frameworks/scraping/providers/htbackdrops.cpp \
    src/utils/Image.cpp \
    src/managers/jukeboxmanager.cpp \
    src/managers/fanartthread.cpp \
    src/frameworks/application/configgroup.cpp \
    src/frameworks/application/configitem.cpp \
    src/managers/settingsmanager.cpp \
    src/gui/guidialog.cpp \
    src/gui/guidialogprogress.cpp \
    src/frameworks/system/task/task.cpp \
    src/views/builderview.cpp \
    src/media/ArtistItem.cpp \
    src/media/AlbumItem.cpp \
    src/frameworks/scraping/providers/theaudiodb.cpp \
    src/media/FanartItem.cpp \
    src/settings/SettingItem.cpp \
    src/managers/CatalogsManager.cpp \
    src/views/FanartView.cpp \
    src/managers/FanartManager.cpp \
    src/frameworks/scraping/providers/Provider.cpp \
#    src/managers/PFCManager.cpp \
#    src/media/PFCFile.cpp \
#    src/utils/Java.cpp \
    src/managers/QRCFileBuilder.cpp \
    src/media/SongItem.cpp \
    src/frameworks/scraping/providers/plexusartist.cpp \
    src/managers/coverthread.cpp \
    src/frameworks/scraping/providers/plexusalbum.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    src/utils/Image.h \
    src/core/Application.h \
    src/database/musicdatabase.h \
    src/database/databasebuilder.h \
#    src/core/AppSettings.h \
    src/core/ThreadManager.h \
    src/core/CollectionManager.h \
    src/frameworks/scraping/providers/htbackdrops.h \
    src/managers/jukeboxmanager.h \
    src/managers/fanartthread.h \
    src/frameworks/application/configgroup.h \
    src/frameworks/application/configitem.h \
    src/managers/settingsmanager.h \
    src/frameworks/stdafx.hpp \
    src/views/guiview.h \
    src/gui/guidialog.h \
    src/gui/guidialogprogress.h \
    src/frameworks/system/task/task.h \
    src/frameworks/system/task/iTask.h \
    src/views/builderview.h \
    src/media/ArtistItem.h \
    src/media/AlbumItem.h \
    src/frameworks/scraping/providers/theaudiodb.h \
    src/media/FanartItem.h \
    src/settings/SettingItem.h \
    src/managers/CatalogsManager.h \
    src/views/FanartView.h \
    src/managers/FanartManager.h \
    src/frameworks/scraping/providers/Provider.h \
#    src/managers/PFCManager.h \
#    src/media/PFCFile.h \
#    src/utils/Java.h \
    src/managers/QRCFileBuilder.h \
    src/media/SongItem.h \
    src/frameworks/scraping/providers/plexusartist.h \
    src/managers/coverthread.h \
    src/frameworks/scraping/providers/plexusalbum.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    qml/albumbuilder/PageAlbum.qml \
    android/libjava.so \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/version.xml \
    android/Makefile \
    android/READ-THIS-BEFORE-MANUALLY-ADDING-FILES-TO-PACKAGE.txt \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java

SUBDIRS += \
    android/java.pro
