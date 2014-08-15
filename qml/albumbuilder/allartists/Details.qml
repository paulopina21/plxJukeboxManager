import QtQuick 2.2
import "../../components"

GridView
{
  id: grdAlbums
  property int idArtist: -1

  cellWidth: 130
  cellHeight: 50
  flow: GridView.TopToBottom
  clip: true

  verticalLayoutDirection: GridView.TopToBottom
  anchors.fill: parent;

  model: application.builderView.albums(idArtist);

  delegate
  {
    Item
    {
      width: cellWidth
      height: cellHeight
      AsyncImage
      {
        id: imgCover
        width: 45; height: 45
        source: cover;
        fallback: "qrc:/images/album.jpg"
        fillMode: Image.Stretch
      }

      ShadowText
      {
        id: txtTitle
        width: 75
        anchors
        {
          top: parent.top
          topMargin: 5
          bottom: parent.bottom
          left: imgCover.right
          leftMargin: 5
        }

        elide: Text.ElideRight
        wrapMode: Text.WordWrap

        text: title
        color: "#0062C6"
        font.family: "Roboto"
        font.weight: Font.Light
        font.pointSize: 10

      }
    }
  }



}
