import QtQuick 2.2

Item
{
  id: switchRoot
  property alias text: textItem.text
  property bool checked: false
  property string onText: "On"
  property string offText: "Off"

  QtObject
  {
    id: priv
    property alias checkedPriv: switchRoot.checked
  }

  width: 200
  height: 30

  MouseArea
  {
    anchors.fill: switchItem
    onClicked:
    {
      switchRoot.checked = !switchRoot.checked
    }
  }

  ShadowText
  {
    id: textItem
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left
    anchors.leftMargin: 22
    anchors.right: switchItem.left
    elide: Text.ElideRight
    pixelSize: 20
  }

  Rectangle
  {
    id: switchItem
//    anchors.fill: switchBackgroundImage

    anchors
    {
      top: switchRoot.top;
      topMargin: (switchRoot.height - height)/2;
      right: switchRoot.right;
      rightMargin: 10;
    }

    height: 25;
    width: 60;

    color: "gray";
    border.color: "white";
    clip: true;

    Rectangle
    {
      id: switchOnImage
      color: "#FFF0C600"
      y: parent.border.width;
      width: parent.width - (parent.border.width * 2);
      height: parent.height - (parent.border.width * 2);
      anchors.right: knob.right;
      opacity: knob.x / switchItem.width;
    }

    ShadowText
    {
      anchors.verticalCenter: parent.verticalCenter
      anchors.right: knob.left
      anchors.left: parent.left

      opacity: (knob.x + knob.width) / switchItem.width;

      horizontalAlignment: Text.AlignHCenter;

      color: "#000000"
      font.pixelSize: 18
      text: onText
    }

    ShadowText
    {
      anchors.verticalCenter: parent.verticalCenter
      anchors.left: knob.right;
      anchors.right: parent.right;

      opacity: 1 - ((knob.x + knob.width) / switchItem.width);

      horizontalAlignment: Text.AlignHCenter;

      color: "#ffffff"
      font.pixelSize: 18
      text: offText
    }

    Rectangle
    {
      id: knob
      x: (checked)? parent.width - width - parent.border.width : 0
      opacity: 0.5
      color: (switchRoot.checked)? "black" : "white";

      width: switchItem.width * 0.15;
      height: switchItem.height - (parent.border.width * 2);

      y: parent.border.width;

      MouseArea
      {
        anchors.fill: parent

        drag
        {
          target: knob;
          axis: Drag.XAxis;
          minimumX: 0;
          maximumX: switchItem.width - width;
        }

        onClicked: checked = !checked
        onReleased: priv.releaseSwitch();
      }
      Behavior on x {
        NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }
      }

    }

  }
}
