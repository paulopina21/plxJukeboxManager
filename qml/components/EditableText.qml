import QtQuick 2.2

TextEdit
{
  id: editText

//  parent: flick;

  // TODO: MAKE FLICK THE ROOT
  // MAKE THE PROPERTIES ALIAS

  property bool editable: false;

  signal accept(string text);
  signal cancel(string text);

  clip: true;

  width: 100;
  height: 62;
  state: "NORMAL";

  font.bold: true;
  font.capitalization: Font.AllUppercase;
  font.family: "Roboto [Condensed]";
  font.pointSize: 16;
  font.weight: Font.Light;

  horizontalAlignment: TextEdit.AlignLeft;
  verticalAlignment: TextEdit.AlignVCenter;

  textFormat: TextEdit.PlainText;
//  wrapMode: TextEdit.Wrap;

  selectedTextColor: "black";
  selectionColor: "gray";

  textMargin: 2;

  states:
      [
    State
    {
      name: "NORMAL";
      PropertyChanges
      {
        target: background;
        color: "transparent";
        border.color: "transparent";
      }
      PropertyChanges
      {
        target: editText;
        readOnly: true;
        selectByMouse: false;
        activeFocusOnPress: false;
        color: "white";
        cursorVisible: false;
        cursorPosition: -1;
        focus: false;
      }
    },
    State
    {
      name: "EDIT";
      PropertyChanges
      {
        target: background;
        color: "black";
        border.color: "white";
      }
      PropertyChanges
      {
        target: editText;
        readOnly: false;
        selectByMouse: true;
        activeFocusOnPress: true;
        color: "white";
        cursorVisible: true;
        cursorPosition: 0;
        focus: true;
      }
    }
  ]


  Keys.onReturnPressed: { accept(editText.text); editText.state = "NORMAL"; event.accepted = true; }
  Keys.onEscapePressed: { cancel(editText.text); editText.state = "NORMAL"; event.accepted = true; }
  onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

  Rectangle
  {
    id: background
    anchors.fill: parent;
    z: -1;
  }


  Flickable
  {
    id: flick

    width: editText.width; height: editText.height;
    contentWidth: editText.paintedWidth
    contentHeight: editText.paintedHeight
    clip: true

    function ensureVisible(r)
    {
      if (contentX >= r.x)
        contentX = r.x;
      else if (contentX+width <= r.x+r.width)
        contentX = r.x+r.width-width;
      if (contentY >= r.y)
        contentY = r.y;
      else if (contentY+height <= r.y+r.height)
        contentY = r.y+r.height-height;
    }

  }

  MouseArea
  {
    enabled: editable;
    anchors.fill: parent;

    onDoubleClicked:
    {
      if (editable) editText.state = "EDIT";
    }
  }

}
