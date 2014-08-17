import QtQuick 2.2
import QtGraphicalEffects 1.0


Rectangle
{
  id: container_dialog;

  property alias title: text_title.text;
  property alias progress: text_progress.text;
  property variant blurTarget;
  property int iProgress;

  function setBlurTarget(target)
  {
    blurTarget = target;
  }

  width: screenGeometry.width;
  height: screenGeometry.height;
  color: "#CC000000";
  opacity: 0;

  signal ok;
  signal cancel;

  Component.onCompleted:
  {
    opacity = 1;
    forceActiveFocus();
    console.log("PROGRESS DIALOG COMPLETED");
  }

  Component.onDestruction: opacity = 0;

  Keys.onEnterPressed: button_ok.clicked();
  Keys.onReturnPressed: button_ok.clicked();

  Behavior on opacity { PropertyAnimation{ duration: 300 } }

  Timer
  {
    id: timer_progress
    interval: 100; running: true; repeat: true;
    onTriggered:{
//      container_dialog.iProgress++;
      container_dialog.iProgress = application.progress;
      if (container_dialog.iProgress == 100)
      {
        button_ok.enabled = true;
        timer_progress.stop();
      }
    }
  }

  FastBlur
  {
    visible: true
    anchors.fill: container_dialog
    source: blurTarget
    radius: 32
  }

  Rectangle
  {
    id: dialog

    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter

    width: parent.width/4; height: parent.height/8

    border.color: "white"
    color: "#70000000"

    opacity: 0;

    Component.onCompleted: opacity = 1;

    Behavior on opacity
    {
      SequentialAnimation
      {
        PauseAnimation { duration: 300 }
        PropertyAnimation{ duration: 250 }
      }
    }

    SpecialText
    {
      id: text_title
      anchors.left: dialog.left
      anchors.leftMargin: 0

      anchors.right: dialog.right
      anchors.rightMargin: 0

      anchors.bottom: dialog.top
      anchors.bottomMargin: 0
      text: ""
    }

    Rectangle{
      id: progressBar
      anchors.left: parent.left
      anchors.leftMargin: 20

      anchors.right: parent.right
      anchors.rightMargin: 20

      anchors.top: parent.top
      anchors.topMargin: 20

      height: 40
      color:"#00000000"

      border.color: "#ffffff"
      border.width: 1

      Rectangle{
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: parent.border.width

        width: ((parent.width / 100) * container_dialog.iProgress) - (anchors.margins*2)
        color: "#00AA00"
      }

      SpecialText
      {
        id: text_progress
        anchors.left: parent.left
        anchors.leftMargin: 5

        anchors.right: parent.right
        anchors.rightMargin: 5

        anchors.top: parent.top
        anchors.topMargin: 5

        anchors.bottom: container_buttons.top
        anchors.bottomMargin: 5

        wrapMode: Text.WordWrap

        text: container_dialog.iProgress + "%"
        textHAlignment: Text.AlignHCenter
      }
    }

    Item
    {
      id: container_buttons
      height: 30

      anchors.left: parent.left
      anchors.leftMargin: 20
      anchors.right: parent.right
      anchors.rightMargin: 20
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 20

      Button
      {
        id: button_ok
        width: 70
        text: "OK"
        horizontalAlignment: Text.Center
        anchors.right: parent.right

        enabled: false
        visible: enabled

        onClicked:
        {
          if (enabled){
            ok();
            loader_dialog.source = "";
            loader_main.forceActiveFocus();
          }
        }

      }
    }
  }
}
