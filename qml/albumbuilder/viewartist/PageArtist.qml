import QtQuick 2.2
import views 1.0
import "../../components"
import "../"

Item
{
  id: container;

  property variant artist

  Component.onCompleted:
  {
    menuToolbar.title.text = artist.name
  }

//  states:[
//    State
//    {
//      name:"FULL"
//      PropertyChanges { target: text_artistName; pixelSize: 40; }
//      PropertyChanges { target: text_artistName; anchors.leftMargin: 280; }
////      PropertyChanges { target: content; anchors.leftMargin: 290; }
//      PropertyChanges { target: albumsGrid; interactive: true; }
//    },
//    State
//    {
//      name:"SMALL"
//      PropertyChanges { target: text_artistName; pixelSize: 30; }
//      PropertyChanges { target: text_artistName; anchors.leftMargin: 20; }
////      PropertyChanges { target: content; anchors.leftMargin: 20; }
//      PropertyChanges { target: albumsGrid; cellWidth: 100; cellHeight: 100; }
//      PropertyChanges { target: albumsGrid; interactive: false; }
//    }
//  ]

//  Panel
//  {
//    height: 40
//    anchors
//    {
//      top: parent.top
//      topMargin: 40
//      left: parent.left
//      leftMargin: 260
//      right: parent.right
//    }

//    ShadowText
//    {
//      id: txtTitle
//      anchors
//      {
//        top: parent.top
//        topMargin: 5
//        left: parent.left
//        leftMargin: 10
//      }

//      elide: Text.ElideRight
//      text: artist.name
//      color: "#0062C6"
//      font.family: "Roboto"
//      font.weight: Font.Light
//      font.pointSize: 30
//    }

//  }

//  SpecialText
//  {
//    id: text_artistName
//    anchors.top: parent.top
//    anchors.left: parent.left
//    anchors.leftMargin: 280

//    editable: true

//    text: artist.name;

//    pixelSize: 40
//    onAccept:
//    {
//      application.builderView.setArtistName(artist.name, text);
//      saveArtist(artist.name);
//    }
//  }


//  Rectangle
//  {
//    id: content
//    width: parent.width// - 300
//    height: parent.height /2.5
//    anchors.left: parent.left
//    anchors.leftMargin: 290
//    anchors.bottom: parent.bottom
//    color: "transparent"

    GridView
    {
      id: albumsGrid
      clip: true
      height: parent.height
      width: parent.width
//      anchors.centerIn: parent
      cellWidth: 168; cellHeight: 165

      anchors
      {
        top: parent.top;
        topMargin: 50
        bottom: parent.bottom;
        bottomMargin: 10;
        left: parent.left;
        leftMargin: 270
        right: parent.right;
      }


//      flow: GridView.BottomToTop
      //    layoutDirection: Qt.LeftToRight
          verticalLayoutDirection: GridView.BottomToTop
      //    layoutDirection: parent.direction
      model: application.builderView.albums(artist.id);

//      delegate: AlbumItem
//      {
//        title: model.title;
//        cover: model.cover;
//        state: container.state;

//        onClicked:
//        {
//          albumsGrid.currentIndex = index;

//          console.debug("Loading idAlbum: "+id);
////          page_album.albumModel   = model;
////          application.builderView.loadSongs(album.idAlbum);

//          list_content.incrementCurrentIndex();
//        }

//      }

      delegate: Card
      {
        title.text: model.title
        title.font.pointSize: 11
        image.source: model.cover
        image.fallback: "qrc:/images/album.jpg"
        image.fillMode: Image.Stretch
//        state: container.state
        width: 162
        height: 160


        MouseArea
        {
          anchors.fill: parent;

          onClicked:
          {
            albumsGrid.currentIndex = index;

            console.debug("Loading idAlbum: "+id);
            ldrPageAlbum.setSource( "PageAlbum.qml", { "albumItem": model } );

            list_content.incrementCurrentIndex();
          }
        }


      }

    }
//  }
}
