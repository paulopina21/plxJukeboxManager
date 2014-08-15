import QtQuick 2.2
import views 1.0
import "../../components"
import "../"

Item
{
  id: container_songs
  property variant albumItem

  anchors.fill: parent;

  Component.onCompleted:
  {
    menuToolbar.title.text = albumItem.artist + " - " + albumItem.title
  }

  Panel
  {

    anchors
    {
      top: parent.top
      topMargin: 50
      left: parent.left
      leftMargin: 270
      right: parent.right
//      rightMargin: 10
      bottom: parent.bottom
      bottomMargin: 10
    }

    AsyncImage
    {
      id: image_album_cover

      height: 160
      width: 160
      anchors
      {
        top: parent.top
        topMargin: 10
        left: parent.left
        leftMargin: 10
      }

      fillMode: Image.Stretch

      source: albumItem.cover
      fallback: "qrc:/images/album.jpg"
    }

    ShadowText
    {
      id: text_albumArtist
      anchors.top: image_album_cover.top
      anchors.left: image_album_cover.right
      anchors.leftMargin: 10

      text: albumItem.artist
      editable: true

//      onAccept:
//      {
//        application.builderView.setArtistName(albumModel.album.artist, text);
//        saveAlbum(albumItem.album);
//      }

//      color: "#ECBF8B"
      color: "#0062C6"
      font.family: "Roboto"
      font.weight: Font.Light
      font.pointSize: 30

    }

    ShadowText
    {
      id: text_albumTitle
      anchors.top: text_albumArtist.bottom
      anchors.topMargin: 10
      anchors.left: image_album_cover.right
      anchors.leftMargin: 10

      text: albumItem.title
      editable: true

//      color: "#ECBF8B"
      color: "#0062C6"
      font.family: "Roboto"
      font.weight: Font.Light
      font.pointSize: 30

//      onAccept:
//      {
//        application.builderView.setAlbumTitle(albumItem.album, text);
//        saveAlbum(albumItem.album);
//      }
    }

    ShadowText
    {
      id: text_albumYear
      anchors.top: text_albumTitle.bottom
      anchors.topMargin: 10
      anchors.left: image_album_cover.right
      anchors.leftMargin: 10

      text: albumItem.year
      editable: true

      pixelSize: 34

      color: "#0062C6"
      font.family: "Roboto"
      font.weight: Font.Light
      font.pointSize: 30

//      onAccept:
//      {
//        application.builderView.setAlbumYear(albumItem.album, text);
//        saveAlbum(albumItem.album);
//      }
    }
//  }
//  Panel
//  {

//    anchors
//    {
//      top: parent.top
//      topMargin: 50
//      left: parent.left
//      leftMargin: 270
//      right: parent.right
//      rightMargin: 10
//      bottom: parent.bottom
//      bottomMargin: 10
//    }
  //    color: "#70000000"

  //    height: 200;

    ListView
    {
      id: list_songs
      clip: true
      anchors
      {
        top: image_album_cover.bottom
        topMargin: 10
        left: parent.left
        right: parent.right
        rightMargin: 10
        bottom: parent.bottom
        bottomMargin: 10
      }

      delegate: itemDelegate
      highlightFollowsCurrentItem: true
      highlight: Rectangle { color: "#F0C600"; }
      Component.onCompleted: currentIndex = 0;
      model: application.builderView.songs(albumItem.id)
    }

    Component
    {
      id: itemDelegate
      ListItem
      {
        id: item
        height: (list_songs.currentIndex == index?90:60)
        clip: true

        onClicked:
        {
          list_songs.currentIndex = index;
          list_songs.forceActiveFocus();
        }

        ShadowText
        {
          id: text_songTrack
          x: 0
          width: 60
          text: model.track;

          anchors.verticalCenter: parent.verticalCenter
          textHAlignment: Text.AlignHCenter

          editable: true
//          onAccept:
//          {
//            application.builderView.setSongTrack(song, text);
//            saveSong(song);
//          }

          onClicked:
          {
            item.click();
          }

          color: "#0062C6"
          font.family: "Roboto"
          font.weight: Font.Light
          font.pointSize: 30

//          pixelSize: 46
        }
        ShadowText
        {
          id: text_songTitle
          text: model.title;

          anchors
          {
            top: parent.top
            topMargin: 3
            left: text_songTrack.right
            right: text_songLenght.left
          }

          editable: true
//          onAccept:
//          {
//            application.builderView.setSongTitle(song, text);
//            saveSong(song);
//          }

          onClicked:
          {
            item.click();
          }

          color: "#0062C6"
          font.family: "Roboto"
          font.weight: Font.Light
          font.pointSize: 23
          elide: Text.ElideRight
        }
//        SpecialText
//        {
//          id: text_songArtist
//          anchors.left: text_songTrack.right
//          anchors.top: text_songTitle.bottom
//          text: artist.name
//          shadowEnabled: false
//          editable: false
//          pixelSize: 16
//          visible: list_songs.currentIndex == index
//        }
        ShadowText
        {
          id: text_songGenre
          anchors.left: text_songTrack.right
          anchors.top: text_songTitle.bottom//text_songArtist.bottom
          text: model.genre
          shadowEnabled: false
//          pixelSize: 16
          visible: list_songs.currentIndex == index

          editable: true
//          onAccept:
//          {
//            application.builderView.setSongGenre(song, text);
//            saveSong(song);
//          }

          color: "#0062C6"
//          font.family: "Roboto"
//          font.weight: Font.Light
          font.pointSize: 12
        }
        ShadowText
        {
          id: text_songFilePath
          x: 0
          anchors.left: text_songTrack.right
          anchors.right: text_songLenght.left
          anchors.rightMargin: 20

          text: model.filePath;
          anchors.bottom: parent.bottom
          anchors.bottomMargin: 7

          shadowEnabled: false
          elide: Text.ElideMiddle

          color: "#0062C6"
//          font.family: "Roboto"
//          font.weight: Font.Light
          font.pointSize: 12        }

        ShadowText
        {
          id: text_songLenght
          x: 0
          width: 80
          text: model.length;
          anchors.right: parent.right
          anchors.rightMargin: 10
          anchors.verticalCenter: parent.verticalCenter
          textHAlignment: Text.AlignRight

          pixelSize: 24
          color: "#0062C6"
        }


      }

    }

  }

}
