import QtQuick 2.2
import "../../components"

Item {
  function setFocus(){
    listSettings.currentIndex = 0;
    listSettings.forceActiveFocus();
    listMenu.focused = false;
  }
  function loseFocus(){
    listSettings.currentIndex = -1;
  }

  ListView
  {
    id: listSettings
    anchors.fill:parent

    clip: true
    model: settingsModel
    delegate: settingsDelegate

    highlightFollowsCurrentItem: true
    highlight: Rectangle {width: parent.width; color: "black";}

    Component.onCompleted: {
      currentIndex = -1;
    }

    Keys.onDownPressed: {
      incrementCurrentIndex();
    }

    Keys.onUpPressed: {
      decrementCurrentIndex();
    }
    Keys.onEscapePressed: {
      currentIndex = -1;
      listMenu.forceActiveFocus();
      listMenu.focused = true;
    }
    Keys.onLeftPressed: {
      currentIndex = -1;
      listMenu.forceActiveFocus();
      listMenu.focused = true;
    }
  }

  //APPLICATION SETTINGS - Menu Model
  ListModel {
    id: settingsModel
    ListElement { title: "Show Jukebox Light"; configName: "showLighting"; type:"bool" }
    ListElement { title: "Show Jukebox Fog"; configName: "showFogParticles"; type:"bool" }
    ListElement { title: "Show Jukebox Cover Reflects"; configName: "showCoverReflect"; type:"bool" }
  }

  Component {
    id: component_text
    Item{
      SpecialText {
        anchors{
          top: parent.top
          bottom: parent.bottom
          left: parent.left
          leftMargin: 22
        }

        pixelSize: config.scalerSize(20)
      }
      SpecialText {
        anchors{
          top: parent.top
          bottom: parent.bottom
          right: parent.right
          rightMargin: 22
        }

        pixelSize: config.scalerSize(18)
        shadowEnabled: false
        onAccept:{
          config.visualConfig[configName] = text;
        }
      }
    }
  }

  Component {
    id: component_switch
    Switch {
      property string configName
      onCheckedChanged: {
        config.visualConfig[configName] = checked;
      }
    }
  }


  Component
  {
    id: settingsDelegate
    ListItem {
      id: list_item_settings
      height: config.scalerSize(80)

      Loader{ id: loader_item; width: parent.width; height: parent.height;}

      Keys.onReturnPressed: {
        switch(type){
        case "label":
          break
        case "text":
          loader_item.item.children[1].state = "EDITING";
          break
        case "int":
          loader_item.item.children[1].state = "EDITING";
          break
        case "bool":
          loader_item.item.checked = !loader_item.item.checked;
          break
        }
      }

      Component.onCompleted:
      {
        switch(type){
        case "label":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          loader_item.item.children[1].text = config.visualConfig[configName];
          break
        case "text":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          loader_item.item.children[1].text = config.visualConfig[configName];
          loader_item.item.children[1].editable = true;
          break
        case "int":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          loader_item.item.children[1].text = config.visualConfig[configName];
          loader_item.item.children[1].editable = true;
          break
        case "bool":
          loader_item.sourceComponent = component_switch;
          loader_item.item.text = title;
          loader_item.item.width = loader_item.width;
          loader_item.item.height = loader_item.height;
          loader_item.item.height = loader_item.height;
          loader_item.item.configName = configName;
          loader_item.item.checked = config.visualConfig[configName];
          break
        }
      }

      ListSeparator{anchors.bottom: parent.bottom}

      onClicked:
      {
        listSettings.currentIndex = index;
        listSettings.forceActiveFocus();
        listMenu.focused = false;
      }
    }
  }

}
