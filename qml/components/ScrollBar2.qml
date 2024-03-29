import QtQuick 2.2

Rectangle
{
  id: scrollBar
  property variant target

  clip: true
//  color: "#80B3B3B3"
  color: "white"
  border.color: "white"
//  color: "#FFF0C600"
  width: 10
  radius: 0
  smooth: true

  anchors.top: target.top
  anchors.bottom: target.bottom
  anchors.right: target.right
  anchors.topMargin: 5
  anchors.bottomMargin: 5
  anchors.rightMargin: 15

  //visible: (track.height == slider.height) ? false : true //TODO: !visible -> width: 0 (but creates a binding loop)


  states: [
    State { name: "nothing"; },
    State { name: "disabled"; when: track.height == slider.height }
  ]

  transitions: [
    Transition
    {
      to: "disabled"; //reversible: true;
      SequentialAnimation
      {
        NumberAnimation { target: scrollBar; property: "opacity"; to: 0; duration: 500; }
        PropertyAction { target: scrollBar; property: "visible"; value: false; }
      }
    },
    Transition
    {
      to: "*";
      PropertyAction { target: scrollBar; property: "visible"; value: true; }
      NumberAnimation { target: scrollBar; property: "opacity"; to: 1; duration: 500; }
    }
  ]

  Timer
  {
    property int scrollAmount

    id: timer
    repeat: true
    interval: 20
    onTriggered:
    {
      target.contentY = Math.max(0, Math.min(target.contentY + scrollAmount, target.contentHeight - target.height));
    }
  }

  Item
  {
    id: track
    anchors.top: parent.top
    anchors.bottom: parent.bottom;
    width: parent.width

    MouseArea
    {
      anchors.fill: parent
      onPressed:
      {
        timer.scrollAmount = target.height * (mouseY < slider.y ? -1 : 1) // scroll by a page
        timer.running = true;
      }
      onReleased:
      {
        timer.running = false;
      }
    }

    Rectangle
    {
      id:slider
//      color: "#343434"
//      color: "#70FFFFFF"
      color: "#FFF0C600";
      smooth: true
      width: parent.width
      radius: 0
      y: Math.max(0,Math.min(track.height-30, target.visibleArea.yPosition * track.height));

      height:
      {
        if (target.visibleArea.yPosition<0)         // Oberer Rand
          Math.max(30, Math.min(target.height / target.contentHeight * track.height, track.height-y) +target.height * target.visibleArea.yPosition)
        else                                        // Mittelbereich
          Math.min(target.height / target.contentHeight * track.height, track.height-y)
      }

      anchors.bottom: (target.visibleArea.yPosition > 1)? parent.bottom: undefined

      MouseArea
      {
        anchors.fill: parent
        drag.target: parent
        drag.axis: Drag.YAxis
        drag.minimumY: 0
        drag.maximumY: track.height - height

        onPositionChanged:
        {
          if (pressedButtons == Qt.LeftButton) {
            target.contentY = slider.y * target.contentHeight / track.height
          }
        }
      }
    }
  }

}
