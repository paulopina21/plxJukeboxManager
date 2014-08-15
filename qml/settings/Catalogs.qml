import QtQuick 2.2
import "../components"
import "application"

Item {
  id: backgroundItem
  clip: true;
  property int i;

  function setFocus(){
    listMenu.forceActiveFocus();
    listMenu.focused = true;
    listMenu.currentIndex = 0;
    listContent.currentIndex = -1;
  }

  function loadData(){
    config.loadSettings();
    contentModel.clear();
    contentModel.append({title: "Source Path",                configName: "source",       type:"text",  value: config.catalogConfig[listMenu.currentIndex]["source"]});
    contentModel.append({title: "Destination Path",           configName: "outputPath",   type:"text",  value: config.catalogConfig[listMenu.currentIndex]["outputPath"]});
    contentModel.append({title: "Automatic Scan at Startup",  configName: "autoRebuild",  type:"bool",  value: config.catalogConfig[listMenu.currentIndex]["autoRebuild"]});
  }

  //APPLICATION SETTINGS
  Rectangle
  {
    id: menu
    width: ((parent.width*0.2) > 250?(parent.width*0.2):250)

    anchors
    {
      left: parent.left;
      top: parent.top;
      bottom: parent.bottom;
      leftMargin: 30;
      rightMargin: 30;
    }

    color: "#80000000";

    ListView
    {
      id: listMenu

      property bool focused: true

      anchors.fill:parent
      anchors.topMargin: config.scalerSize(40)

      model: application.databaseManager.catalogs();
      delegate: menuDelegate
      clip: true

      highlightFollowsCurrentItem: true

      highlight: Rectangle {
        width: parent.width;
        color: listMenu.focused?"black":"#80000000";
      }

      onCurrentIndexChanged:
      {
        loadData();
      }

      Component.onCompleted:
      {
        currentIndex = 0;
        forceActiveFocus();
      }

      Keys.onDownPressed:
      {
        incrementCurrentIndex();
        listContent.currentIndex = -1
      }

      Keys.onUpPressed:
      {
        decrementCurrentIndex();
        listContent.currentIndex = -1
      }

      Keys.onReturnPressed:
      {
        listContent.currentIndex = 0;
        listMenu.focused = false;
        listContent.forceActiveFocus();
        loadData();
      }

      Keys.onRightPressed:
      {
        listContent.currentIndex = 0
        listMenu.focused = false
        listContent.forceActiveFocus()
      }

    }
    Rectangle
    {
      anchors
      {
        top: parent.top
        left: parent.left
        right: parent.right
      }

      height: config.scalerSize(40)
      color: "#80000000"

      ShadowText
      {
        anchors.fill: parent
        anchors.leftMargin: 20
        pixelSize: config.scalerSize(16)
        text: "Catalogs"
      }
    }

    //APPLICATION SETTINGS - Menu Delegate
    Component
    {
      id: menuDelegate
      ListItem {
        height: config.scalerSize(50)

        SpecialText
        {
          text: model.name
          x: 22
          pixelSize: config.scalerSize(20)
          height: config.scalerSize(50)
        }

        ListSeparator{anchors.bottom: parent.bottom}

        onClicked:
        {
          listMenu.focused = true;
          listMenu.forceActiveFocus();
          listMenu.currentIndex = index;

          listContent.currentIndex = -1;
          listContent.currentIndex = -1;
        }
      }
    }

  }

  //== CONTENT ==//
  Rectangle
  {
    width: parent.width - menu.width - 90
    height: (parent.height / 2) - 15;

    anchors
    {
      right: parent.right;
      top: parent.top;
      leftMargin: 30;
      rightMargin: 30;
    }

    color: "#80000000"
    clip: true

    Rectangle
    {
      id: title

      anchors
      {
        top: parent.top
        left: parent.left
        right: parent.right
      }

      height: config.scalerSize(40)
      color: "#80000000"
//      clip: true

      ShadowText
      {
        anchors.fill: parent
        anchors.leftMargin: 20
        pixelSize: config.scalerSize(16)
        text: "Settings"
      }
    }

    ListModel
    {
      id: contentModel
    }

    ListSettings{
      id: listContent;
      anchors.fill: parent;

      contentModel: contentModel;
      listMenu: listMenu;
      nextBottomComponent: listMenu;
      nextRightComponent: listMenu;
    }
  }

}
