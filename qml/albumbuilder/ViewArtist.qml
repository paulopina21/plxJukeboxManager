import QtQuick 2.2
import QtGraphicalEffects 1.0
import media 1.0
import views 1.0
import "../components"
import "viewartist"
//import "ViewArtist.js" as ViewArtist
import "../utils.js" as Utils

Item
{
  id: viewArtist
  anchors.fill: parent
  property int artistIndex
  property variant artist

  clip: true;

  function saveArtist(artist)
  {
    application.mainModel.saveArtist(artist);
  }
  function saveAlbum(album)
  {
    application.mainModel.saveAlbum(album);
  }
  function saveSong(song)
  {
    application.mainModel.saveSong((song));
  }

//  states:[
//    State {
//      name:"FULL"
//      PropertyChanges { target: list_content; anchors.topMargin: 50; }
//    },
//    State {
//      name:"SMALL"
//      PropertyChanges { target: list_content; anchors.topMargin: 10; }
//    }
//  ]

//  Component.onCompleted: menuToolbar.title.text = artist.name

//  Image
//  {
//    id: image_fanart
//    x: -list_content.currentIndex*parent.width/3.0
//    y: (parent.height - height)/2

//    width: parent.width + parent.width/3.0
//    height: parent.height + parent.height/3.0

//    source: artist.fanart

//    Behavior on x { PropertyAnimation{ easing.type: Easing.OutSine; duration: 400 } }
//  }

  Image
  {
    id: imgFanart
    x: -20; y: -20;
    width: parent.width +20; height: parent.height +20;
    //      anchors.fill: parent
    //      anchors.topMargin: 35
    //      anchors.verticalCenter: parent.verticalCenter

    asynchronous: true;

    fillMode: Image.PreserveAspectCrop
    source: artist.fanart;

    onStatusChanged:
    {
//      if (status == Image.Error)
//        source = "qrc:/images/music.jpg"
    }

    SequentialAnimation on y
    {
      loops: Animation.Infinite
      NumberAnimation { to: 0; duration: Utils.getRandom(3500,15000); }
      PauseAnimation { duration: Utils.getRandom(100,10000); }
      NumberAnimation { to: -20; duration: Utils.getRandom(3500,15000); }
    }

    SequentialAnimation on x
    {
      loops: Animation.Infinite
      NumberAnimation { to: 0; duration: Utils.getRandom(3500,15000); }
      PauseAnimation { duration: Utils.getRandom(100,10000); }
      NumberAnimation { to: -20; duration: Utils.getRandom(3500,15000); }
    }


  }

  ListView
  {
    id: list_content

    anchors
    {
      top: parent.top
//      topMargin: menuT
      left: parent.left
      right: parent.right
      bottom: parent.bottom
    }

    interactive: false

    model: itemModel
    preferredHighlightBegin: 0;
    preferredHighlightEnd: 0;
    highlightRangeMode: ListView.StrictlyEnforceRange;
    orientation: ListView.Horizontal;
    snapMode: ListView.SnapOneItem;
    highlightMoveVelocity: 1500;
    cacheBuffer: 200
  }


  VisualItemModel
  {
    id: itemModel

    PageArtist
    {
      id:page_artist
      width: list_content.width; height: list_content.height
      state: viewArtist.state;

      artist: viewArtist.artist;
    }

    Loader
    {
      id: ldrPageAlbum
      asynchronous: true;
      width: list_content.width; height: list_content.height
    }
  }

}
