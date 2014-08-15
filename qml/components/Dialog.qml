import QtQuick 2.2
import QtGraphicalEffects 1.0


Rectangle
{
  id: container_dialog;

  property alias title: text_title.text;
  property alias content: text_content.text;
  property variant blurTarget;

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
    console.log("DIALOG COMPLETED");
  }

  Component.onDestruction: opacity = 0;

  Keys.onEnterPressed: button_ok.clicked();
  Keys.onReturnPressed: button_ok.clicked();

  Behavior on opacity { PropertyAnimation{ duration: 300 } }

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

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: parent.width /2; height: parent.height / 3

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

    SpecialText
    {
      id: text_content
      anchors.left: parent.left
      anchors.leftMargin: 10

      anchors.right: parent.right
      anchors.rightMargin: 10

      anchors.top: parent.top
      anchors.topMargin: 10

      anchors.bottom: container_buttons.top
      anchors.bottomMargin: 10

      wrapMode: Text.WordWrap

      text: ""
      textHAlignment: Text.AlignHCenter
    }

    Item
    {
      id: container_buttons
      height: 30

      anchors.left: parent.left
      anchors.leftMargin: 10
      anchors.right: parent.right
      anchors.rightMargin: 10
      anchors.bottom: parent.bottom

      Button
      {
        id: button_ok
        width: 100
        text: "OK"
        anchors.right: parent.right

        onClicked:
        {
          ok();
          loader_dialog.source = "";
          loader_main.forceActiveFocus();
        }

      }
    }
  }
}
