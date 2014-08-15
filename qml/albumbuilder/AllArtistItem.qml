import QtQuick 2.2
import music 1.0
import views 1.0
import "../components"
import "../utils.js" as Utils

Item
{
  id: itemDelegate

  property int artistIndex
//  property int idArtist
//  property string artist
//  property string fanart
  property variant albumsModel
  property variant artist;

  width: parent.width
  height: width /2.5

  Component.onCompleted: {
    application.builderView.loadAlbums(artist.id);
    albumsModel = application.builderView.list(BuilderView.AlbumList);
    console.debug("Loading AllArtistItem: "+albumsModel.album.title);
  }

  ListItem
  {
    anchors.fill: parent
    hoverEnabled: false;
    //Fanart
    Rectangle
    {
      id: container_fanart;
      clip: true;
      anchors.fill: parent
      anchors.topMargin: 35
      anchors.verticalCenter: parent.verticalCenter
      color: "transparent"

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
          if (status == Image.Error)
            source = "qrc:/images/music.jpg"
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
    }

    //Artist Name
    SpecialText
    {
      x: 20; y: 0
      text: artist.name;

      pixelSize: 40
      shadowEnabled: true
      asynchronous: true
    }

    //Albums Grid
    Rectangle
    {
      height: 170

      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom

      anchors.bottomMargin: 30
      anchors.leftMargin: 30
      anchors.rightMargin: 30

      color: "transparent"

      GridView
      {
        id: albumsGrid
        clip: true
        anchors.fill: parent

        cellWidth: config.scalerSize(180); cellHeight: config.scalerSize(180);

        model: albumsModel

        delegate: AlbumItem {
          rotationAngle: 0
          title: model.album.title
          cover: model.album.cover.thumb
          onClicked:
          {
//            albumsGrid.currentIndex = index;
//            text_albumArtist.text = album.artist;
//            text_albumTitle.text = album.title;
//            text_albumYear.text = album.year;
//            image_album_cover.source = album.cover.full;
//            list_songs.model = album.songs;
//            list_content.incrementCurrentIndex();
          }

          Component.onCompleted:
          {
            rotationAngle = 0;
          }
        }

      }

    }

  }

}
