import QtQuick 2.2

Rectangle
{
  id: container_edit

  clip: true
  width: 200; height: 30;
  color: "#80000000"
  border.color: "white"

  signal clicked

  property alias text: edit.text
  property string initialText
  property alias iconSource: image_icon.source
  property alias horizontalAlignment: edit.horizontalAlignment
  property alias capitalization: edit.font.capitalization
  property alias readOnly: edit.readOnly

  TextEdit
  {
    id: edit
    width: 200
    color: "white"
    selectedTextColor: "black"
    selectionColor: "white"

//    anchors.centerIn: parent
    anchors.fill: parent
    anchors.margins: 4
    horizontalAlignment: TextEdit.AlignLeft
    verticalAlignment: TextEdit.AlignVCenter

    selectByMouse: true    

    text: text != ""? text : initialText;

    onFocusChanged: {
      if (activeFocus)
        text == initialText? text = "": text = container_edit.text;
      else
        text == ""? text = initialText: text = text;
    }

    font.family: "Oswald"
    font.bold: true
    font.pixelSize: 12
    font.letterSpacing: 1
    font.capitalization: Font.AllUppercase
  }

  Image
  {
    id: image_icon
    width: 20; height: 20;
    anchors.top: parent.top
    anchors.bottom: parent.bottom    
    anchors.right: container_edit.right

    anchors.margins: 4

//    source: "qrc:/images/ui/search.png"
    visible: (source != "")
  }

  MouseArea
  {
    anchors.fill: image_icon
    visible: image_icon.visible
    hoverEnabled: true
    propagateComposedEvents: true;
    cursorShape: Qt.PointingHandCursor;

    onClicked: {
      container_edit.clicked();
    }
  }
}
