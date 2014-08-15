import QtQuick 2.2
import QtGraphicalEffects 1.0

import "components"

//    passo o dia passo a noite cheirando suvaco
//    o seu peido fede tanto to até suado
//    seu chulé ta azendando tô até com azia
//    e esse bafo de cavalo? vá escovar na pia!
//    IÊEEEEE IÊEEEE

Rectangle
{
  id: container_main
  objectName: "container_main"
  width: screenGeometry.width;
  height: screenGeometry.height;
  color: "black"

  ConfigInfo { id: config; objectName: "config"}

  function showDialog(msg, title)
  {
    loader_dialog.source = "components/Dialog.qml";
    loader_dialog.item.content = msg;
    loader_dialog.item.title = title;
    loader_dialog.item.setBlurTarget(loader_main);
  }

  Image
  {
    id: background
    anchors.fill: parent
    source: "qrc:/images/backgrounds/plexus.jpg"
  }

//  Loader
//  {
//    id: loader_bg
//    anchors.fill: parent
//    asynchronous: true;
//    opacity: 0.1
//    source: "components/DynamicBackground.qml"
//    Component.onCompleted: setSource("components/DynamicBackground.qml", {"flyingTitles" : false })
//  }

//  FastBlur
//  {
//    visible: true
//    anchors.fill: loader_bg
//    source: loader_bg
//    radius: 64
//  }

//  GaussianBlur
//  {
//    id: blur
//    anchors.fill: loader_bg
//    source: loader_bg
//    radius: 32
//    samples: 16

//  }  

  Loader
  {
    id: loader_main
    objectName: "loader_main"
    anchors.fill: parent
    focus: true
    source: "Loading.qml"
  }

  Loader
  {
    id: loader_dialog;
    objectName: "loader_dialog";
    anchors.fill: parent;
    focus: false;
  }

//  Connections
//  {
//    target: application
//    onLoading: {
//      loader_main.source = "Loading.qml";
//      console.log("APP LOADING...");
//    }
//    onLoaded:{
//      console.log("APP LOADED!");
//      if (application.mainModel.isCollectionEmpty())
//      {
//        loader_main.source = "ViewEmpty.qml";
//      }
//      else
//      {
//        loader_main.source = "Loading.qml"
//        application.load();
//      }
//    }
//  }

}
