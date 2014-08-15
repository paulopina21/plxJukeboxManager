import QtQuick 2.2

Panel
{
  width: 230;
  height: 400;

  property alias title: txtTitle
  property alias image: imgContent
  property alias details: ldrDetails

  ShadowText
  {
    id: txtTitle
    anchors
    {
      top: parent.top
      topMargin: 5
      left: parent.left
      leftMargin: 10
      right: parent.right
      rightMargin: 10
    }

    elide: Text.ElideRight

    color: "#0062C6"
  }

  AsyncImage
  {
    id: imgContent
    anchors
    {
      top: txtTitle.bottom
      topMargin: 5
      left: parent.left
      leftMargin: 9
      right: parent.right
      rightMargin: 9
      bottom: ldrDetails.top
      bottomMargin: 4
    }

    source: model.fanart
  }

  Loader
  {
    id: ldrDetails
    asynchronous: true
    anchors
    {
      left: parent.left
      leftMargin: 9

      right: parent.right
      rightMargin: 9

      bottom: parent.bottom
      bottomMargin: 1
    }
  }

}
