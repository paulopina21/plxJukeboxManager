import QtQuick 2.2
import QtGraphicalEffects 1.0
import "../components"

Item {
  width: screenGeometry.width
  height: screenGeometry.height
  x: 0

  Component.onCompleted:
  {
    listMenu.currentIndex = 0;
    listContent.currentIndex = 0;
    listContent.model.children[listContent.currentIndex].setFocus();

    menuToolbar.state = "SETTINGS";
    menuToolbar.strCurrentWindow = qsTr("settings");
  }

//  Image
//  {
//    id: background

//    anchors.verticalCenter: parent.verticalCenter
//    width: parent.width * 1.5
//    x: (-listContent.currentIndex) * ((width - parent.width)/listContent.count)
//    y: 0

//    source: "qrc:/images/backgrounds/settings.jpg"
//    fillMode: Image.PreserveAspectCrop

//    Behavior on x { PropertyAnimation{ easing.type: Easing.OutSine; duration: 400 } }
//  }

  //=================================================================
  //                        SETTINGS PAGES
  //=================================================================
  ListView
  {
    id: listContent
    anchors
    {
      top: menuButtons.bottom
      topMargin: 20
      left: parent.left
      right: parent.right

      bottom: container_dialog_buttons.top
    }
    model: mgrModel
    preferredHighlightBegin: 0; preferredHighlightEnd: 0
    highlightRangeMode: ListView.StrictlyEnforceRange
    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem; flickDeceleration: 2000
    cacheBuffer: 200

    onCurrentIndexChanged: {
      listContent.model.children[listContent.currentIndex].setFocus();
      listMenu.currentIndex = listContent.currentIndex
    }

    Keys.onLeftPressed: {
      listContent.decrementCurrentIndex();
    }

    Keys.onRightPressed: {
      listContent.incrementCurrentIndex();
    }

    Keys.onEscapePressed: {
      loader_main.source = "../albumbuilder/MainView.qml"
    }
    Keys.onPressed: {
      if (event.key === Qt.Key_Home && menuToolbar.activate_plxAlbumBuilder){
        loader_main.source = "../albumbuilder/MainView.qml"
      }
    }
  }

  // Pages Model
  //============
  VisualItemModel
  {
    id: mgrModel
    Catalogs {width: listContent.width; height: listContent.height;}
  }

  //=================================================================
  //                        SETTINGS MENU
  //=================================================================
  Rectangle {
    id: menuButtons
    anchors
    {
      left: parent.left
      top: parent.top
      topMargin: 60
      leftMargin: 30
      rightMargin: 30
    }

    anchors.horizontalCenter: parent.horizontalCenter
    height: config.scalerSize(30)

    color: "#40000000"
    clip: true
    visible: listMenu.count > 0

    ListView
    {
      id: listMenu

      anchors.fill:parent
      anchors.leftMargin: 150
      anchors.rightMargin: 150

      model: menu_mgrModel
      delegate: menuDelegate

      orientation: ListView.Horizontal

      highlightFollowsCurrentItem: true
      highlight: Rectangle {
        color: "black";
      }

    }

    // Menu Model
    //===========
    ListModel {
      id: menu_mgrModel
      ListElement { title: "Catalogs";}
    }

    // Menu Delegate
    //==============
    Component
    {
      id: menuDelegate
      ListItem {
        height: btnChoice.height
        width: btnChoice.width + 100
        Button
        {
          id: btnChoice
          text: qsTr(title)

          anchors.left: parent.left
          anchors.leftMargin: 50

          border.color:"transparent"
          width: config.scalerSize(100);
          onClicked: {
            listMenu.currentIndex = index;
            listContent.currentIndex = index;
            listContent.forceActiveFocus();
          }
        }
      }
    }

  }

  //=================================================================
  //                        ACTION BUTTONS
  //=================================================================
  Rectangle
  {
    id: container_dialog_buttons

    anchors.bottom: parent.bottom
    anchors.bottomMargin: parent.height / (1.618*6)

    anchors.right: parent.right
    anchors.rightMargin: 30

    width: childrenRect.width; height: config.scalerSize(30);
    color: "transparent"

    Button
    {
      id: button_cancel
      color: "#80000000"
      text: "Cancel"
      width: config.scalerSize(100)

      onClicked: {
        loader_main.source = "../albumbuilder/MainView.qml"
      }
    }

    Button
    {
      id: button_save
      x: button_cancel.x + button_cancel.width + 20;
      width: config.scalerSize(100)

      color: "#80000000"
      text: "Save"

      onClicked: {
        config.saveSettings();
      }

    }

  }

  MenuToolbar{
    id: menuToolbar;
    anchors.top: parent.top;
    anchors.left: parent.left;
    anchors.right: parent.right;
  }
}
