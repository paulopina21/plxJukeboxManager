import QtQuick 2.0

Item{
  property var contentModel;
  property var listMenu;
  property var nextBottomComponent;
  property var nextRightComponent;
  property alias currentIndex: listContent.currentIndex;

  function forceActiveFocus(){
    listContent.forceActiveFocus();
  }

  ListView
  {
    id: listContent
    anchors.fill: parent
    anchors.topMargin: config.scalerSize(40)

    clip: true
    model: contentModel
    delegate: contentDelegate

    highlightFollowsCurrentItem: true
    highlight: Rectangle {width: listContent.width; color: "black";}

    Component.onCompleted:
    {
      currentIndex = -1;
    }

    Keys.onDownPressed:
    {
      if (currentIndex < count-1)
        incrementCurrentIndex();
      else{
        currentIndex = -1;
        nextBottomComponent.forceActiveFocus();
        nextBottomComponent.focused = true;
      }
    }

    Keys.onUpPressed:
    {
      decrementCurrentIndex();
    }

    Keys.onEscapePressed:
    {
      currentIndex = -1;
      listMenu.forceActiveFocus();
      listMenu.focused = true;
    }
    Keys.onLeftPressed:
    {
      currentIndex = -1;
      listMenu.forceActiveFocus();
      listMenu.focused = true;
    }
    Keys.onRightPressed:
    {
      currentIndex = -1;
      nextRightComponent.forceActiveFocus();
      nextRightComponent.focused = true;
    }
  }

  //CONTENT DELEGATE
  Component
  {
    id: contentDelegate
    ListItem {
      id: list_item_settings
      height: config.scalerSize(50)

      Loader{ id: loader_item; width: parent.width; height: 50;}

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
        switch(type)
        {
        case "label":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          if (configName)
            loader_item.item.children[1].text = config.catalogConfig[listMenu.currentIndex][configName];
          break
        case "text":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          if (configName)
          {
            loader_item.item.children[1].text = config.catalogConfig[listMenu.currentIndex][configName];
            loader_item.item.children[1].editable = true;
          }
          break
        case "int":
          loader_item.sourceComponent = component_text;
          loader_item.item.children[0].text = title;
          if (configName){
            loader_item.item.children[1].text = config.catalogConfig[listMenu.currentIndex][configName];
            loader_item.item.children[1].editable = true;
          }
          break
        case "bool":
          loader_item.sourceComponent = component_switch;
          loader_item.item.text = title;
          loader_item.item.width = loader_item.width;
          loader_item.item.height = 50;

          if (configName)
          {
            loader_item.item.configName = configName;
            loader_item.item.checked = config.catalogConfig[listMenu.currentIndex][configName];
          }
          break
        }
      }

      ListSeparator{anchors.bottom: parent.bottom}

      onClicked:
      {
        listContent.currentIndex = index;
        listContent.forceActiveFocus();
        listMenu.focused = false;
      }
    }
  }

  //COMMON COMPONENTS
  Component {
    id: component_text
    Item{
      SpecialText
      {
        anchors
        {
          top: parent.top
          bottom: parent.bottom
          left: parent.left
          leftMargin: 22
        }

        pixelSize: config.scalerSize(20)
      }
      SpecialText
      {
        anchors
        {
          top: parent.top
          bottom: parent.bottom
          right: parent.right
          rightMargin: 22
        }

        pixelSize: config.scalerSize(18)
        shadowEnabled: false
        onAccept:
        {
          config.catalogConfig[listMenu.currentIndex][configName] = text;
        }
      }
    }
  }

  Component
  {
    id: component_switch
    Switch
    {
      property var configName
      onCheckedChanged:
      {
        config.catalogConfig[listMenu.currentIndex][configName] = checked;
      }
    }
  }
}
