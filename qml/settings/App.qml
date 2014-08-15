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
    listContent.currentIndex = 0;

    for (i=0; i<contentModel.count;i++ ){
      contentModel.children[i].loseFocus();
    }
  }

  //APPLICATION SETTINGS
  Rectangle {
    id: menu
    width: ((parent.width*0.2) > 250?(parent.width*0.2):250)
    anchors{
      left: parent.left
      top: parent.top
      bottom: parent.bottom
    }
    anchors.leftMargin: 30
    anchors.rightMargin: 30

    color: "#80000000"
    clip: true

    ListView
    {
      id: listMenu

      property bool focused: true

      anchors.fill:parent
      model: menuModel
      delegate: menuDelegate

      highlightFollowsCurrentItem: true
      highlight: Rectangle {
        width: parent.width;
        color: listMenu.focused?"black":"#80000000";
      }

      Component.onCompleted: {
        currentIndex = 0;
        forceActiveFocus();
      }

      Keys.onDownPressed: {
        incrementCurrentIndex();
        listContent.currentIndex = currentIndex

        for (i=0; i<contentModel.count;i++ ){
          contentModel.children[i].loseFocus();
        }
      }

      Keys.onUpPressed: {
        decrementCurrentIndex();
        listContent.currentIndex = currentIndex

        for (i=0; i<contentModel.count;i++ ){
          contentModel.children[i].loseFocus();
        }
      }

      Keys.onReturnPressed: {
        contentModel.children[currentIndex].setFocus();
      }
      Keys.onRightPressed: {
        contentModel.children[currentIndex].setFocus();
      }

    }

    //APPLICATION SETTINGS - Menu Model
    ListModel {
      id: menuModel
      ListElement { title: "General Settings"; }
      ListElement { title: "Visual Settings"; }
      ListElement { title: "Input Settings"; }
      ListElement { title: "Hardware Settings"; }
    }

    //APPLICATION SETTINGS - Menu Delegate
    Component
    {
      id: menuDelegate
      ListItem {
        height: config.scalerSize(50)

        SpecialText {
          text: title
          x: 22
          pixelSize: config.scalerSize(20)
          height: config.scalerSize(50)
        }

        ListSeparator{anchors.bottom: parent.bottom}

        onClicked:
        {
          listMenu.forceActiveFocus();
          listMenu.focused = true;
          listMenu.currentIndex = index;
          listContent.currentIndex = index;

          for (i=0; i<contentModel.count;i++ ){
            contentModel.children[i].loseFocus();
          }
        }
      }
    }

  }

  //CONTENT
  Rectangle {
    width: parent.width - menu.width - 90
    anchors{
      right: parent.right
      top: parent.top
      bottom: parent.bottom
    }
    anchors.leftMargin: 30
    anchors.rightMargin: 30

    color: "#80000000"
    clip: true

    ListView
    {
      id: listContent
      anchors.fill: parent

      model: contentModel
      preferredHighlightBegin: 0; preferredHighlightEnd: 0
      highlightRangeMode: ListView.StrictlyEnforceRange
      orientation: ListView.Vertical
      snapMode: ListView.SnapOneItem; flickDeceleration: 2000
      cacheBuffer: 200
    }

    VisualItemModel
    {
      id: contentModel

      General{width: listContent.width; height: listContent.height}
      Visual{width: listContent.width; height: listContent.height}
      Input{width: listContent.width; height: listContent.height}
      Hardware{width: listContent.width; height: listContent.height}
    }

  }


}
