import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle
{
  id: container_button
//  clip: true
  height: config.scalerSize(30)
  width: config.scalerSize(55)
  color: "transparent"
  border.color: "white"

  signal clicked;
  signal pressed;
  signal entered;
  signal exited;

//  property alias glyph: image_glyph.source
  property alias text: text_caption.text
  property alias pixelSize: text_caption.font.pixelSize
  property alias textColor: text_caption.color
  property alias horizontalAlignment: text_caption.horizontalAlignment

  property alias glyph: image_glyph;
  property alias caption: text_caption;

//  property bool hoverEffect: true

  property color originalColor: "transparent";
  property color originalTextColor: "#0062C6";

  property int textLeftMargin: 10;

//  Behavior on color { ColorAnimation{ duration: 150 } }

  function hasGlyph()
  {
    return image_glyph.status === Image.Ready;
  }

  Image
  {
    id: image_glyph

    anchors
    {
      left: parent.left;
      leftMargin: 5;

      top: parent.top;
      topMargin: 5;

      bottom: parent.bottom;
      bottomMargin: 5;

      right: parent.right;
      rightMargin: 5;
    }

    onStatusChanged:
    {
      if (status === Image.Ready)
      {

      }
    }

//    width: status === Image.Ready? 32:0;
//    height: status === Image.Ready? 32:0;
    fillMode: Image.PreserveAspectFit;
  }

  ShadowText
  {
    id: text_caption
    height: parent.height

    anchors
    {
      left: hasGlyph()? image_glyph.right: parent.left;
      leftMargin: textLeftMargin
      right: parent.right;
      rightMargin: 10;

      top: parent.top;
      bottom: parent.bottom;
    }       

    onTextChanged:
    {
      if (text !== "")
      {
        glyph.anchors.right = undefined;
        glyph.anchors.rightMargin = 0;
      }
      else
      {
        glyph.anchors.right = parent.right;
        glyph.anchors.rightMargin = 5;
      }

    }

    color: "#0062C6"
//    font.family: "Roboto"
//    font.weight: Font.Light

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft

    elide: Text.ElideRight
    wrapMode: Text.Wrap

    font.pointSize: 12
  }

  ColorOverlay
  {
    id: effColor;
    x: image_glyph.x; y: image_glyph.y;
    width: image_glyph.width; height: image_glyph.height;
    visible: true;
    source: image_glyph
    color: "black"
  }


  MouseArea
  {
    hoverEnabled: true
    anchors.fill: container_button
    propagateComposedEvents: true;
    cursorShape: Qt.PointingHandCursor;

    onEntered:
    {
      originalColor = container_button.color;
      originalTextColor = text_caption.color;

      text_caption.color = "black";
      container_button.color = "#DDF0C600";

//      effColor.visible = true;

      container_button.entered();
    }

    onExited:
    {
      container_button.color = originalColor;
      text_caption.color = originalTextColor;

//      effColor.visible = false;

      container_button.exited();
    }


    onClicked:
    {
      container_button.color = "#80F0C600";
//      container_button.color = originalColor;
      text_caption.color = originalTextColor;

//      effColor.visible = false;
      container_button.clicked();
    }

//    onReleased:
//    {
//      container_button.color = originalColor;
//      text_caption.color = originalTextColor;
//      container_button.clicked();
//    }

  }

}
