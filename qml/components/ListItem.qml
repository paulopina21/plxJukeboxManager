import QtQuick 2.2

Rectangle
{
  id: container_item
  width: parent.width
  height: 30
  color: "transparent";
  property alias hoverEnabled: mousearea_delegate.hoverEnabled

  signal clicked;

//  Behavior on color { ColorAnimation{ duration: 255 } }
  Behavior on height { PropertyAnimation{ duration: 300 } }

  MouseArea
  {
    id: mousearea_delegate
    anchors.fill: parent
    hoverEnabled: true;

    onEntered:
    {
      container_item.color = "#80F0C600";
    }

    onExited:
    {
      container_item.color = "transparent";
    }

    onClicked:
    {
      container_item.clicked();
    }
  }

}
