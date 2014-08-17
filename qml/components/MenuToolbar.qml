import QtQuick 2.2

Panel
{
  id: container_toolbar
  anchors.top: parent.top
  height: 40

  property string strCurrentWindow: "plxAlbumBuilder"
  property string currentLoadedWindow: ""
  property bool activate_plxAlbumBuilder: false
  property alias title: txtTitle

  signal allArtists

  Behavior on color { ColorAnimation{ duration: 255 } }

  states: [
    State
    {
      name: "LOADING"
      PropertyChanges
      {
        target: container_toolbar
//        color: "transparent"
      }
    },

    State
    {
      name: "ALBUMBUILDER"
      PropertyChanges
      {
        target: container_toolbar
//        color: "#70000000"
      }
    },

    State
    {
      name: "SETTINGS"
      PropertyChanges
      {
        target: container_toolbar
//        color: "#70000000"
      }
    }
  ]

  state: "LOADING";


//  Image
//  {
//    anchors
//    {
//      top: parent.top;
//      topMargin: 5;

//      left: parent.left;
//      leftMargin: 40

//      bottom: parent.bottom;
//      bottomMargin: 5;
//    }

//    source: "qrc:/images/plexus_white.png"
//    fillMode: Image.PreserveAspectFit;

//  }


  Button
  {
    id: btnAllArtists
    width: 260
    anchors
    {
      top: parent.top
      left: parent.left
      bottom: parent.bottom
    }
//    anchors.right: parent.right;
    border.color: "transparent"
    glyph.source: "qrc:/images/ui/view-artist.png"
    text: "All Artists"

    onClicked:
    {
      allArtists();
    }

  }


  ShadowText
  {
    id: txtTitle
    anchors
    {
      top: parent.top
      topMargin: 2
      left: parent.left
      leftMargin: 270
    }

    elide: Text.ElideRight
    color: "#0062C6"
    font.family: "Roboto"
    font.weight: Font.Light
    font.pointSize: 30
  }

  Button
  {
    id: button_home
    anchors.right: parent.right;
    anchors.rightMargin: 20;
    height: parent.height;
    width: parent.height;
    border.color: "transparent"
    visible: container_toolbar.state == "ALBUMBUILDER"

    glyph.source: "qrc:/images/ui/home.png"
    glyph.height: 32;
    glyph.width: 32;

    onClicked:
    {
      loader_main.source = "../ViewEmpty.qml";
    }
  }


  Button
  {
    id: button_buildAlbums
    anchors.right: button_home.left
    height: parent.height
    width: parent.height;
    border.color: "transparent"
    visible: container_toolbar.state == "ALBUMBUILDER"

    glyph.source: "qrc:/images/ui/save.png"
    glyph.height: 32;
    glyph.width: 32;

    onClicked:
    {
      application.mainModel.buildAlbums();
    }
  }

  Button
  {
    id: button_scanAlbums
    anchors.right: button_buildAlbums.left
    height: parent.height
    width: parent.height;
    border.color: "transparent"
    visible: container_toolbar.state == "ALBUMBUILDER"

    glyph.source: "qrc:/images/ui/refresh.png"
    glyph.height: 32;
    glyph.width: 32;

    onClicked:
    {
//      application.rebuildDatabase()
      application.mainModel.listAlbums();
      console.log("Listing Albums")
    }
  }

//  Button
//  {
//    id: button_Settings
//    anchors.right: button_home.right;
//    anchors.rightMargin: 20;
//    height: parent.height;
//    width: parent.height;
//    border.color: "transparent"
//    visible: container_toolbar.state == "ALBUMBUILDER"

//    glyph.source: "qrc:/images/ui/settings.png"
//    glyph.height: 32;
//    glyph.width: 32;

//    onClicked:
//    {
//      // passa state pro settings
//      container_toolbar.state = "SETTINGS"
//      loader_main.source = "../settings/MainView.qml";
//    }
//  }

//  Button
//  {
//    id: button_albumBuilder
//    anchors.right: parent.right;
//    anchors.rightMargin: 20;
//    height: parent.height;
//    width: parent.height;
//    border.color: "transparent"
//    visible: container_toolbar.state == "SETTINGS"

//    glyph.source: "qrc:/images/ui/back.png"
//    glyph.height: 32;
//    glyph.width: 32;

//    onClicked:
//    {
//      container_toolbar.state = "ALBUMBUILDER"
//      loader_main.source = "../albumbuilder/MainView.qml";
//    }
//  }


//  EditField {
//    id: edit_search
//    anchors.right: parent.right
//    anchors.verticalCenter: parent.verticalCenter
//    anchors.rightMargin: 15

//    visible: container_toolbar.state == "ALBUMBUILDER"

//    initialText: qsTr("Search In Library")
//  }
}
