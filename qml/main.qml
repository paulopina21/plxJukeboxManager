import QtQuick 2.2
import QtGraphicalEffects 1.0
import QtQuick.Window 2.1

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
  height: screenGeometry.height -20;
  color: "black"

  ConfigInfo { id: config; objectName: "config"}

  function showDialog(msg, title)
  {
    loader_dialog.source = "qrc:/qml/components/Dialog.qml";
    loader_dialog.item.content = msg;
    loader_dialog.item.title = title;
    loader_dialog.item.setBlurTarget(loader_main);
  }

  function showProgressDialog(title)
  {
    loader_dialog.source = "qrc:/qml/components/ProgressDialog.qml";
    loader_dialog.item.title = title;
    loader_dialog.item.setBlurTarget(loader_main);
    console.log("TESTE 9")
  }

  function showSearchProgress(message, content, progress, total)
  {
    searchProgress.strMessage = message;
    searchProgress.strContent = content;
    searchProgress.iProgress  = (progress * 100) / total;
    console.log(searchProgress.strMessage, searchProgress.strContent, searchProgress.iProgress);
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
    source: "qrc:/qml/Loading.qml"
  }

  Rectangle{
    id: searchProgress

    property string strMessage;
    property string strContent;
    property int iProgress;

    anchors.top: parent.top
    anchors.right: parent.right
    anchors.topMargin: 50
    anchors.rightMargin: 40

    color: "black"

    width: 300
    height: 50

    opacity: (iProgress == 100) ? 0 : (iProgress <= 0) ? 0 : 0.4

    Behavior on opacity { PropertyAnimation{ duration: 300 } }

    ShadowText{
      id: searchProgress_message
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 2
      anchors.leftMargin: 5

      text: searchProgress.strMessage
      wrapMode: Text.WordWrap
      textHAlignment: Text.AlignHCenter
      pixelSize: 10
    }

    Rectangle{
      id: searchProgress_bar
      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.right: parent.right
      anchors.rightMargin: 5

      anchors.top: searchProgress_message.bottom
      anchors.topMargin: 2

      height: 15
      color:"#000000"

      border.color: "#ffffff"
      border.width: 1

      Rectangle{
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: parent.border.width

        width: ((parent.width / 100) * searchProgress.iProgress) - (anchors.margins*2)
        color: "#00AA00"
        Behavior on width { PropertyAnimation{ duration: 300 } }
      }

      ShadowText
      {
        id: searchProgress_percent
        anchors.fill: parent

        textHAlignment: Text.AlignHCenter
        textVAlignment: Text.AlignVCenter
        pixelSize: 10

        text: searchProgress.iProgress + "%"
      }
    }

    ShadowText{
      id: searchProgress_content
      anchors.top: searchProgress_bar.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.topMargin: 2
      anchors.leftMargin: 5

      text: searchProgress.strContent
      pixelSize: 10
    }

    MouseArea{
      anchors.fill: parent;
      hoverEnabled: true;
      onEntered: parent.opacity = (parent.iProgress == 100) ? 0 : (parent.iProgress <= 0) ? 0 : 0.8
      onExited: parent.opacity = (parent.iProgress == 100) ? 0 : (parent.iProgress <= 0) ? 0 : 0.4
    }
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
