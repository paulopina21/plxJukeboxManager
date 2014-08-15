import QtQuick 2.2
import "../components"
import "../effects"
import views 1.0
import QtGraphicalEffects 1.0

Item
{
  id: container_main

  property variant artistModel

  width: screenGeometry.width;
  height: screenGeometry.height;

  Component.onCompleted:
  {
//    artistModel = application.builderView.list(BuilderView.ArtistList);
//    menuArtists.show();
    menuToolbar.state = "ALBUMBUILDER"
    loader_hud.source = "MenuArtists.qml"
    loader_content.source = "allartists/AllArtists.qml"
  }

  Loader
  {
    id: loader_content
    anchors.fill: parent
//    source: "../components/DynamicBackground.qml"
//    Component.onCompleted: setSource("../components/DynamicBackground.qml", {"flyingTitles" : false })
//    onLoaded: item.flyingTitles = false;
  }

//  Loader
//  {
//    id: effectLoader
//    anchors.fill: menuArtists
//    source: "../effects/EffectGaussianBlur.qml"
//  }


//  GaussianBlur
//  {
//    id: blur
//    anchors.fill: menuArtists
//    source: loader_albumbuilder
//    radius: 8
//    samples: 16

//  }
  ShaderEffectSource {
      id: theSource
      smooth: true
      hideSource: false
      sourceItem: container_main
  }

  Loader
  {
    id: loader_hud
    width: 260;

    anchors
    {
      top: menuToolbar.bottom
      left: parent.left
      bottom: parent.bottom
    }

    asynchronous: true;
  }


  Connections
  {
    target: loader_hud.item
//    onAllArtists:
//    {
//      loader_content.source = "allartists/AllArtists.qml"
//    }

    onClicked:
    {
      console.log(artist);
      loader_content.setSource("ViewArtist.qml", {"artist" : artist, "state": "FULL" })
    }
  }

  MenuToolbar
  {
    id: menuToolbar;
    anchors.top: parent.top;
    anchors.left: parent.left;
    anchors.right: parent.right;
    onAllArtists: loader_content.source = "allartists/AllArtists.qml"

  }
}
