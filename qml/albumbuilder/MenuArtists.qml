import QtQuick 2.2
//import "ViewArtist.js" as ArtistView
//import "AllArtists.js" as AllArtists
//import music 1.0
import "../components"
import media 1.0

Panel
{
  id: container

  anchors.fill: parent;

//  color: "transparent";
//  color: "#A0292929"

  property bool bOppened: true;
//  property alias model: list_artists.model;
  property int originalX: x;

//  signal allArtists;
  signal clicked(variant artist);

  function isShowing()
  {
    return bOppened;
  }

  function show()
  {
    visible = true
    container.x = originalX;
    container.opacity = 1;
    container.bOppened = true;
  }

  function hide()
  {
    container.originalX = container.x;
    container.x = screenGeometry.width+container.width;
    container.opacity = 0;
    container.bOppened = false;
  }

  Behavior on x { PropertyAnimation{ duration: 350 } }
  Behavior on opacity { PropertyAnimation { duration: 300 } }

//  Button
//  {
//    id: btnAllArtists
//    anchors.left: parent.left;
//    anchors.right: parent.right;
//    height: 40
//    border.color: "transparent"
//    glyph.source: "qrc:/images/ui/view-artist.png"
//    text: "All Artists"

//    onClicked:
//    {
//      allArtists();
//    }

//  }

  ListView
  {
    id: list_artists
    clip: true
    model: application.builderView.artistMenu();

    anchors
    {
      top: parent.top
      left: parent.left
      right: scroll.left
      bottom: parent.bottom;
    }

    delegate: itemDelegate
    highlightFollowsCurrentItem: true
    highlight: Rectangle { color: "#F0C600"; }
    section.property: "name"
    section.criteria: ViewSection.FirstCharacter
    section.delegate: sectionHeading

    Component.onCompleted: {
      currentIndex = -1;
    }
  }

  ScrollBar2
  {
    id: scroll
    target: list_artists
    anchors.right: parent.right;
    anchors.rightMargin: 0;
    anchors.topMargin: 0;
    anchors.bottomMargin: 0;
  }


  // The delegate for each section header
  Component
  {
    id: sectionHeading

    Rectangle
    {
      width: parent.width
      height: 40
      color: "#E4E2DB"
//      color: "#F0C600"

//      Image
//      {
//        id: imgIcon
//        x:5
//        width: 30;
//        height: 30;
//        anchors.verticalCenter: parent.verticalCenter
//        source: "qrc:/images/ui/circle.png"
//        ShadowText
//        {
//          x: 15
//          anchors.fill: parent
//          anchors.verticalCenter: parent.verticalCenter
//          anchors.horizontalCenter: parent.horizontalCenter

//          textHAlignment: Text.AlignHCenter
//          textVAlignment: Text.AlignVCenter

//          text: section
//          pixelSize: 22
//          shadowEnabled: false
//          color: "black"
//        }

//      }
      ShadowText
      {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        textHAlignment: Text.AlignHLeft
        textVAlignment: Text.AlignVCenter

        font.family: "Roboto"
        font.weight: Font.Light

        text: section
        pointSize: 22
        shadowEnabled: false
        color: "black"
      }


    }
  }

  Component
  {
    id: itemDelegate
    Button
    {
      id: btnAllArtists
      anchors.left: parent.left;
      anchors.right: parent.right;
      height: 35
      border.color: "transparent"
      textLeftMargin: 10;
//      glyph: "qrc:/images/ui/view-artist.png"
      text: name

      onClicked:
      {
        container.clicked(model);
      }

    }
  }


}
