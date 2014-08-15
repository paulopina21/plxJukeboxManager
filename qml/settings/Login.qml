import QtQuick 2.2
import "../components"

Item {
  property string strPwd: "";

  Component.onCompleted: {
    loggedIn = false;
    menuToolbar.activate_plxAlbumBuilder = false;
  }

  Rectangle{
    id: loginBox
    height: 150
    width: 400

    color: "#80000000"

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    SpecialText{
      text: "Settings Login"

      anchors.top: parent.top
      anchors.topMargin: 20
      anchors.horizontalCenter: parent.horizontalCenter
      shadowEnabled: false
    }

    Rectangle{
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right

      anchors.leftMargin: 20
      anchors.rightMargin: 20
      anchors.bottomMargin: 20

      height: 60

      color: "black"
      border.color: "white"
      border.width: 1

      SpecialText{
        id: loginText

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 10
        anchors.rightMargin: 10

        anchors.top: parent.top
        anchors.topMargin: -15

        text: ""
        pixelSize: 74
      }
    }
  }

  Keys.onPressed: {
    if (event.key === Qt.Key_Return){
      logIn(strPwd);
      strPwd = "";
      loginText.text = "";
    }
    else{
      strPwd = strPwd + event.key;
      loginText.text = loginText.text + "*";
    }
  }
}
