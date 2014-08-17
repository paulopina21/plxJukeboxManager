import QtQuick 2.2
import QtGraphicalEffects 1.0
import music 1.0
import "../../components"

Item
{
  id: container

  anchors.fill: parent;

  //  color: "transparent"
  //    color: "#FFFA6800"

  Component.onCompleted:
  {
    menuToolbar.title.text = ""
    //    list_artists.update();
  }

  Component.onDestruction:
  {
    //    console.log("ALL ARTIST DESTRUCTION");
  }


  GridView
  {
    id: list_artists
    clip: false
    cellWidth: (width /2) -10; cellHeight: 410
    cacheBuffer: 0;
    anchors
    {
      top: parent.top
      topMargin: 50;
      left: parent.left
      leftMargin: loader_hud.x + loader_hud.width + 10;

      bottom: parent.bottom;
      bottomMargin: 20;

      right: parent.right;
      rightMargin: 10
    }

    Component.onCompleted:
    {
      update();
      currentIndex = -1;
      forceActiveFocus();
    }

    onCountChanged:
    {
      console.debug("# of artists: "+list_artists.count);
      list_artists.update();
    }

    //    interactive: false

    // Laureon: TODO: This is hardcoded, must be calculated
    preferredHighlightBegin: 0; preferredHighlightEnd: cellHeight

    highlightRangeMode: ListView.StrictlyEnforceRange
    snapMode: ListView.SnapToItem; flickDeceleration: 2000

    highlightFollowsCurrentItem: true
    highlight: Rectangle { color: "#000000"; }

    Keys.onDownPressed: list_artists.moveCurrentIndexDown()
    Keys.onUpPressed: list_artists.moveCurrentIndexUp()

    model: application.builderView.artists();

    delegate: Card
    {
      width: list_artists.cellWidth - 10
      title.text: name
      title.font.family: "Roboto"
      title.font.weight: Font.Light

      image.source: fanart
      image.fallback: "qrc:/images/backgrounds/plexus.jpg"
      details.height: 100;

      Component.onCompleted:
      {
        details.setSource("Details.qml", { "idArtist": id } );
      }
    }

  }

  ScrollBar2
  {
    target: list_artists
    anchors.right: parent.right;
    anchors.rightMargin: 15;
    anchors.topMargin: 0;
    anchors.bottomMargin: 0;
  }

  Loader
  {
    id: loaderFullArtist
    asynchronous: true;

    onLoaded: state = "reanchored"

    states: State {
      name: "reanchored"
      AnchorChanges { target: loaderFullArtist; anchors.top: parent.top }
      AnchorChanges { target: loaderFullArtist; anchors.bottom: parent.bottom }
      AnchorChanges { target: loaderFullArtist; anchors.left: parent.left }
      AnchorChanges { target: loaderFullArtist; anchors.right: parent.right }
    }

    transitions: Transition {
      // smoothly reanchor myRect and move into new position
      AnchorAnimation { duration: 1000 }
    }
  }


//  Component
//  {
//    id: delegate
//    Item
//    {
//      anchors.fill: parent
////      width: parent.width
////      height: width /2.5

//      Loader
//      {
//        id: artistLoader
//        asynchronous: true;
//        anchors.fill: parent;

//        states: State {
//          name: "reanchored"
//          AnchorChanges { target: artistLoader; anchors.top: container.top }
//          AnchorChanges { target: artistLoader; anchors.bottom: container.bottom }
//          AnchorChanges { target: artistLoader; anchors.left: container.left }
//          AnchorChanges { target: artistLoader; anchors.right: container.right }
//        }

//        transitions: Transition {
//          // smoothly reanchor myRect and move into new position
//          AnchorAnimation { duration: 1000 }
//        }

//      }

//      Component.onCompleted:
//      {
//        artistLoader.setSource( "ViewArtist.qml", { "artist": model, "state": "SMALL" } );
//      }

//      FastBlur
//      {
//        id: blur
//        visible: false;
//        anchors.fill: artistLoader
//        source: artistLoader
//        radius: 16
//      }



//      MouseArea
//      {
//        hoverEnabled: true;
//        //        preventStealing: true;
//        anchors.fill: parent;
//        propagateComposedEvents: true;
//        onEntered:
//        {
//          hud_controls.visible = true;
//          //          blur.visible = true;
//        }

//        onExited:
//        {
//          hud_controls.visible = false;
//          //          blur.visible = false;
//        }
//      }

//      Rectangle
//      {
//        id: hud_controls
//        visible: false;
//        anchors.fill: parent;
//        color: "#30FFFFFF";
//        focus: true;

//        Rectangle
//        {
//          width: 45;
//          color: "#70000000";
//          anchors
//          {
//            top: parent.top;
//            bottom: parent.bottom;
//            right: parent.right;
//          }

//          Button
//          {
//            id: btnFullscreen;
//            height: 35;
//            width: 35;
//            color: "#70FFFFFF";
//            radius: 6;

//            glyph.source: "qrc:/images/ui/fullscreen.png";

//            anchors
//            {
//              top: parent.top;
//              topMargin: 10;

//              right: parent.right;
//              rightMargin: 5
//            }

//            onClicked:
//            {
//              list_artists.currentIndex = index;
//              loaderFullArtist.x = list_artists.x;
//              loaderFullArtist.y = list_artists.currentItem.y;
//              loaderFullArtist.width = list_artists.currentItem.width;
//              loaderFullArtist.height = list_artists.currentItem.height;
//              loaderFullArtist.setSource( "ViewArtist.qml", { "artist": model, "state": "FULL" } );
//            }

//            onEntered:
//            {
//              hud_controls.visible = true;
//            }

//          }

////          Rectangle
////          {
////            id: rectFullscreen
////            width: 35;
////            height: 35;
////            color: "#70FFFFFF";
////            radius: 6;
////            anchors
////            {
////              top: parent.top;
////              topMargin: 10;

////              right: parent.right;
////              rightMargin: 5
////            }

////            MouseArea
////            {
////              hoverEnabled: true;
////              anchors.fill: parent;

////              onClicked:
////              {
//////                artistLoader.state = "reanchored";

////                list_artists.currentIndex = index;
////                loaderFullArtist.x = list_artists.x;
////                loaderFullArtist.y = list_artists.currentItem.y;
////                loaderFullArtist.width = list_artists.currentItem.width;
////                loaderFullArtist.height = list_artists.currentItem.height;
////                loaderFullArtist.setSource( "ViewArtist.qml", { "artist": model, "state": "FULL" } );
////              }

////            }



////          }

//          Rectangle
//          {
//            width: 35;
//            height: 35;
//            color: "#70FFFFFF";
//            radius: 6;
//            anchors
//            {
//              top: btnFullscreen.bottom;
//              topMargin: 10;

//              //              left: parent.left;
//              //              leftMargin: 3.5;

//              right: parent.right;
//              rightMargin: 5


//              //              horizontalCenter: parent.Center;
//            }
//          }


//          //            Image
//          //            {
//          //              id: ico_fullscreen
//          //              width: 40;
//          //              height: 40;
//          //              source: "qrc:/images/ui/fullscreen.png";
//          //  //            anchors
//          //  //            {
//          //  //              top: parent.top;
//          //  //              topMargin: 20;
//          //  //            }

//          //  //          fillMode: Image.Pad;
//          //  //            anchors.horizontalCenter: parent;
//          //  //            anchors.verticalCenter: parent;


//          //            }
//          //          }

//        }


//      }

//    } // END ITEM


//  } // END COMPONENT

  // Attach scrollbar to the right edge of the view.

} // END ALL ARTIST
