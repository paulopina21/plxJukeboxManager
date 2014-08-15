import QtQuick 2.2
import QtGraphicalEffects 1.0
import music 1.0
import "../components"

//Item {
//Album
//{
//  id:album


//  Image {
//    id: imgCover
//    smooth: true
//    anchors.horizontalCenter: parent.horizontalCenter
//    width: 200
//    height: 200
//    source: album.cover.full

//    onStatusChanged:
//    {
//      if (imgCover.status === Image.Error)
//      {
//        source = "qrc:/images/album.png";
//      }
//    }

//    Rectangle {
//      width: parent.width; height: 40
//      clip: true
//      color: Qt.rgba(0,0,0,0.7)
//      anchors.bottom: parent.bottom

//      Text {
//        anchors.left: parent.left
//        anchors.leftMargin: 5
//        anchors.right: parent.right
//        anchors.rightMargin: 5
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom

//        text: album.title
//        wrapMode: TextEdit.WordWrap
//        color: "white"
//        font.family: "Oswald"
//        font.bold: true
//        font.pixelSize: 12
//        font.letterSpacing: 1
//        font.capitalization: Font.AllUppercase
//      }
//    }
//  }

//}
//}

Item
{
  id: item
  width: GridView.view.cellWidth - 15
  height: GridView.view.cellHeight - 15

  property alias title: text_albumTitle.text
  property alias cover: image_cover.source

  property real rotationAngle: 0

  signal clicked

  Component.onCompleted:
  {
    rotation.angle = 0;
  }

  transform: [
    Rotation {
      id: rotation;
      angle: -90;
      axis { x: 0; y: 1; z:0 }
      origin.x: 0
      Behavior on angle {
        NumberAnimation { duration: 300 }
      }
    }
  ]

  states:[
    State {
      name:"FULL"
      PropertyChanges { target: text_albumTitle; pixelSize: config.scalerSize(14); }
    },
    State {
      name:"SMALL"
      PropertyChanges { target: text_albumTitle; pixelSize: config.scalerSize(10); }
    }
  ]

  Image
  {
    id: image_cover
    //    smooth: true
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.fill: parent
    asynchronous: true

    onStatusChanged:
    {
      if (image_cover.status === Image.Error)
      {
        source = "qrc:/images/album.png";
      }
    }

    Rectangle
    {
      width: parent.width; height: 40
      clip: true
      color: "#70000000"
      anchors.bottom: parent.bottom

      ShadowText
      {
        id: text_albumTitle
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        wrapMode: TextEdit.WordWrap
        pixelSize: config.scalerSize(14);
      }
    }
  }

  MouseArea
  {
    hoverEnabled: true
    anchors.fill: parent
    onEntered:
    {
      rotation.angle = -10;
    }

    onExited:
    {
      rotation.angle = 0;
    }

    onClicked:
    {
      item.clicked();
    }
  }

}

