import QtQuick 2.2
import "components"
import views 1.0
import QtQuick.Dialogs 1.0

Rectangle
{
  anchors.fill: parent
  color: "black"

//  ConfigInfo { id: config }
  Image
  {
    id: image_logo
    y: 100
    height: 100; width: parent.width;
    fillMode: Image.PreserveAspectFit
    source: "qrc:/images/plexus_white_big.png"
  }

  Button
  {
    id: button_close
    anchors.top: parent.top
    anchors.topMargin: 1
    anchors.right: parent.right
    anchors.rightMargin: 10
    width: config.scalerSize(100)

    height: config.scalerSize(40)

    pixelSize: config.scalerSize(20)

    color: "#f0c600"
    textColor: "black"

    text: qsTr("SAIR");
    textLeftMargin: config.scalerSize(27)

    onClicked: Qt.quit();
  }

  Text
  {
    id: title
    x: ((parent.width - width) / 2)

    anchors.top: image_logo.bottom
    anchors.topMargin: 30

    text: qsTr("Olá! Escolha um catálogo para iniciar.")
    textFormat: Text.RichText
    color: "white"

    fontSizeMode: Text.Fit
    font.family: "Oswald"
    font.bold: true
    font.pixelSize: config.scalerSize(32)
    font.letterSpacing: 1
    font.capitalization: Font.AllUppercase
  }

  Rectangle{
    id: btnAddCatalog
    anchors
    {
      top: title.top
      left: title.left
      right: title.right
    }

    anchors.topMargin: 100
    anchors.leftMargin: 150
    anchors.rightMargin: 150

    height: 40
    border.color: "#AAFFFFFF"
    color: "#60FFFFFF"

    clip: true
    Behavior on height { PropertyAnimation{ duration: 300 } }

    Button
    {
      id: add_button
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      height: 40

      border.color: "transparent"
      text: "ADICIONAR NOVO CATÁLOGO"
      textLeftMargin: 60;
      textColor: "#FFFFFF"

      pixelSize: config.scalerSize(25)

      onClicked:{        
        if (parent.height == 40)
          parent.height = 160
        else
          parent.height = 40
      }
    }

    Text
    {
      id: add_namelabel
      anchors.top: add_button.bottom
      anchors.left: parent.left
      anchors.topMargin: 10
      anchors.leftMargin: 4
      width: 110

      text: "NOME:"
      color: "white"
      horizontalAlignment: Text.AlignRight;

      font.family: "Oswald"
      font.pixelSize: config.scalerSize(20)
//      font.letterSpacing: 1
    }

    EditField{
      id: add_name

      anchors.top: add_namelabel.top
      anchors.left: add_namelabel.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10

      KeyNavigation.tab: add_source
      KeyNavigation.priority: KeyNavigation.BeforeItem

      initialText: qsTr("Nome do Catálogo")
    }

    Text
    {
      id: add_sourcelabel
      anchors.top: add_namelabel.bottom
      anchors.left: parent.left
      anchors.topMargin: 10
      anchors.leftMargin: 4
      width: 110

      text: "CAMINHO:"
      color: "white"
      horizontalAlignment: Text.AlignRight;

      font.family: "Oswald"
      font.pixelSize: config.scalerSize(20)
//      font.letterSpacing: 1
    }

    EditField{
      id: add_source

      anchors.top: add_sourcelabel.top
      anchors.left: add_sourcelabel.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10

      KeyNavigation.backtab: add_name
      KeyNavigation.priority: KeyNavigation.BeforeItem

      readOnly: true

      capitalization: Font.MixedCase

      initialText: qsTr("Caminho do Catálogo")
      iconSource: "qrc:/images/ui/search.png"

      onClicked: {
        fileDialog.open();
      }
    }

    Image{
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.margins: 4

      width: 32
      height: 32

      source: "qrc:/images/ui/add.png"
    }

    Button
    {
      id: add_confirm
      anchors.top: add_source.bottom
      anchors.topMargin: 10
      anchors.horizontalCenter: parent.horizontalCenter

      height: 40
      width: 60
      horizontalAlignment: Text.AlignHCenter

      border.color: "transparent"
      text: "OK"
      textColor: "#FFFFFF"

      pixelSize: config.scalerSize(25)

      onClicked:{
        if (add_name.text != add_name.initialText && add_source.text != add_source.initialText){
          console.log("Adding Catalog: "+add_name.text+" - "+add_source.text);          
          application.selectCatalog(application.databaseManager.addCatalog(add_name.text, add_source.text));
          loader_main.source = "Loading.qml"
        }
      }
    }
  }

  FileDialog {      
      id: fileDialog
      selectFolder: true
      title: "Por favor escolha o caminho do catálogo"
      property string dirPath
      onAccepted: {
          dirPath = fileDialog.fileUrl;
          if (dirPath.substring(0,7) == "file://"){
            dirPath = dirPath.substring(7)
          }
          add_source.text = dirPath;
          console.log("SELECTED: "+dirPath);
      }
      onRejected: {
      }
  }

  //********** LIST CATALOGS **********//
  ListView
  {
    id: list_catalogs
    clip: true
    model: application.databaseManager.catalogs();

    anchors
    {
      top: btnAddCatalog.bottom
      left: btnAddCatalog.left
      right: btnAddCatalog.right
      bottom: parent.bottom
    }

    anchors.topMargin: 10
    anchors.bottomMargin: 100

    spacing: 10

    delegate: itemDelegate
    highlightFollowsCurrentItem: true
    highlight: Rectangle { color: "#F0C600"; }

    Component.onCompleted: currentIndex = -1;
  }

  ScrollBar2
  {
    id: scroll
    target: list_catalogs
    anchors.right: list_catalogs.right;
    anchors.rightMargin: -20;
    anchors.topMargin: 0;
    anchors.bottomMargin: 0;
  }

  //********** ITEM DELEGATE **********//
  Component
  {
    id: itemDelegate

    Rectangle{
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 5
      anchors.bottomMargin: 5

      height: 40
      border.color: "#AAFFFFFF"
      color: "#30FFFFFF"

      clip: true
      Behavior on height { PropertyAnimation{ duration: 300 } }

      Button
      {
        id: delegate_button
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 40

        border.color: "transparent"
        text: name
        textLeftMargin: 60;
        textColor: "#FFFFFF"

        pixelSize: config.scalerSize(25)

        onClicked:
        {
          parent.height = (parent.height == 40) ? 120 : 40
        }
      }

      Text
      {
        id: delegate_source
        anchors.top: delegate_button.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5
        anchors.leftMargin: 60

        text: source
        textFormat: Text.RichText
        color: "white"

        fontSizeMode: Text.Fit
        font.family: "Oswald"
        font.pixelSize: config.scalerSize(12)
        font.letterSpacing: 1
        font.capitalization: Font.AllUppercase
      }

      Text
      {
        id: delegate_status
        anchors.top: delegate_source.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5
        anchors.leftMargin: 60

        text: albumCount + " Albuns, " + artistCount + " Artistas, " + songCount + " Mídias " + (autoRebuild ? ", AUTO SCAN":"")
        textFormat: Text.RichText
        color: "white"

        fontSizeMode: Text.Fit
        font.family: "Oswald"
        font.pixelSize: config.scalerSize(12)
        font.letterSpacing: 1
        font.capitalization: Font.AllUppercase
      }

      Button
      {
        id: btn_load
        anchors.top: delegate_status.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -70

        height: 20
        width: 120
        horizontalAlignment: Text.AlignHCenter

        border.color: "transparent"
        text: "CARREGAR"
        textColor: "#FFFFFF"
        color: "#00CC00"

        pixelSize: config.scalerSize(20)

        onClicked:{
          loader_main.source = "Loading.qml"
          application.selectCatalog(idCatalog)
        }
      }

      Button
      {
        id: btn_delete
        anchors.top: delegate_status.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 70

        height: 20
        width: 120
        horizontalAlignment: Text.AlignHCenter

        border.color: "transparent"
        text: "EXCLUIR"
        textColor: "#FFFFFF"
        color: "#CC0000"

        pixelSize: config.scalerSize(20)

        onClicked:{
          console.log("Removendo:"+idCatalog)
          application.databaseManager.deleteCatalog(idCatalog)
        }
      }

      Image{
        id: delegate_icon
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 4

        width: 32
        height: 32

        source: "qrc:/images/ui/music.png"
      }

    }

  }

}
