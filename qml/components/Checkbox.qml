import QtQuick 2.2

Rectangle {
  id: container_checkbox
  width: childrenRect.width + 20
  height: childrenRect.height
  color: "transparent"
  border.color: "white"

  property bool checked: false
  property string caption: "checkbox"

  Rectangle
  {
    id: checkbox
    clip: true
    width: height * (1.618*1.5); height: 20
    anchors.left: parent.left
    anchors.leftMargin: 10
    anchors.verticalCenter: parent.verticalCenter
    color: "#80000000"
    border.color: "white"

    states:[
      State {
        name:"ON"
        AnchorChanges { target:checkbox_switch; anchors.left: undefined; anchors.right: checkbox.right; }
      },
      State {
        name:"OFF"
        AnchorChanges { target:checkbox_switch; anchors.left: checkbox.left; anchors.right: undefined; }
      }
    ]

    transitions: Transition {
      // smoothly reanchor myRect and move into new position
      AnchorAnimation { duration: 200 }
    }
    Rectangle
    {
      id: checkbox_switch
      height: parent.height
      width: parent.width - (parent.width / 1.618)
      color: "white"
    }
    Text
    {
      id: text_off

      anchors.left: checkbox_switch.right
      anchors.leftMargin: 0
      anchors.right: checkbox.right

      text: ""
      color: "white"

      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignHCenter

      anchors.verticalCenter: parent.verticalCenter

      font.family: "Webdings"
      font.bold: true
      font.pixelSize: 15
      font.letterSpacing: 1
      font.capitalization: Font.AllUppercase
    }
    Text
    {
      id: text_on

      anchors.right: checkbox_switch.left
      anchors.leftMargin: 0
      anchors.left: parent.left

      text: ""
      color: "white"

      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignHCenter

      anchors.verticalCenter: parent.verticalCenter

      font.family: "Webdings"
      font.bold: true
      font.pixelSize: 15
      font.letterSpacing: 1
      font.capitalization: Font.AllUppercase
    }
    MouseArea
    {
      anchors.fill: parent
      onClicked:
      {
        container_checkbox.checked = !container_checkbox.checked;
        checkbox.state = container_checkbox.checked? "ON":"OFF";
      }
    }
  }

  Text
  {
    id: textCaption

    anchors.left: checkbox.right
    anchors.leftMargin: 10

    text: qsTr(container_checkbox.caption)
    color: "white"

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    anchors.verticalCenter: parent.verticalCenter
    //    anchors.horizontalCenter: parent.horizontalCenter

    elide: Text.ElideRight
    wrapMode: Text.Wrap

    font.family: "Oswald"
    font.bold: true
    font.pixelSize: 13
    font.letterSpacing: 1
    font.capitalization: Font.AllUppercase
  }


}
