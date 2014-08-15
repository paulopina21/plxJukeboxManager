import QtQuick 2.2
//import QtGraphicalEffects 1.0

Text
{
//  id: container_specialText
//  width: text_text.contentWidth;
//  height: text_text.contentHeight;


  id: text_text
  color: "white"
//  visible: !shadowEnabled;

//  anchors.fill: container_specialText

  textFormat: Text.PlainText;

  verticalAlignment: Text.AlignVCenter;
  horizontalAlignment: Text.AlignLeft;

  elide: Text.ElideNone;
  wrapMode: Text.NoWrap;

  font.bold: true;
  font.family: "Roboto [Condensed]"
  font.weight: Font.Light;
//  font.underline: container_specialText.underline
//  font.pixelSize: 22;
  font.pointSize: 22;
//  font.pixelSize: config.scalerSize(22)
  font.capitalization: Font.AllUppercase

  property alias pixelSize: text_text.font.pixelSize;
  property alias pointSize: text_text.font.pointSize;

//  property alias letterSpacing: text_text.font.letterSpacing
//  property alias capitalization: text_text.font.capitalization
  property alias textVAlignment: text_text.verticalAlignment
  property alias textHAlignment: text_text.horizontalAlignment

//  property alias bold: text_text.font.bold
//  property alias underline: text_text.font.underline

  property bool asynchronous: true

  property bool editable: false

  property alias shadowEnabled: shadow.visible;

  property real shadowRadius: 10
  property real shadowSamples: 16
  property real shadowHOffset: 2
  property real shadowVOffset: 2
  property real shadowSpread: 0.3
  property bool shadowTransparentBorder: false;

  signal clicked;

  onTextChanged: shadow.text = text;

  Text
  {
    id: shadow;
    z: -1;

    visible: false;

    anchors.fill: parent;
    anchors.topMargin: shadowVOffset;
    anchors.leftMargin: shadowHOffset;

    color: "black";

    textFormat: text_text.textFormat;

    verticalAlignment: text_text.verticalAlignment;
    horizontalAlignment: text_text.horizontalAlignment;

    elide: text_text.elide;
    wrapMode: text_text.wrapMode;

    font: text_text.font;
//    font.bold: text_text.font.bold;
//    font.family: text_text.font.family;
//    font.weight: text_text.font.wi


//    font.pixelSize: text_text.font.pixelSize;
//    font.pointSize: text_text.font.pointSize;
//    font.capitalization: text_text.font.capitalization;

    text: text_text.text;

    opacity: 0.75;

//    scale: 1.01;
  }

//  FastBlur
//  {
//    id: blur
//    z: -1
//    anchors.fill: shadow
//    source: shadow;

//    radius: 10;

//  }

//  Component
//  {
//    id: component_shadow

//    DropShadow
//    {
//      id: shadow_text

////      visible: false;

////      source: text_text;
//    }
//  }

//  Loader
//  {
//    id: loader_text_shadow

//    z: -1;

//    focus: false
//    asynchronous: text_text.asynchronous
//    anchors.fill: text_text

//    onLoaded:
//    {
//      item.anchors.fill = text_text;
//      item.source = text_text;
//      item.transparentBorder = text_text.shadowTransparentBorder;
//      item.radius = config.scalerSize(text_text.shadowRadius);
//      item.spread = text_text.shadowSpread;
//      item.samples = text_text.shadowSamples;
//      item.verticalOffset = text_text.shadowVOffset;
//      item.horizontalOffset = text_text.shadowHOffset;
//      item.color = "#80000000";
//      item.visible = true;
//    }
//  }

  Component.onCompleted:
  {
//    shadow.visible = shadowEnabled;
//    if (shadowEnabled)
//      loader_text_shadow.sourceComponent = component_shadow;
  }

  onShadowEnabledChanged:
  {
//    shadow.visible = shadowEnabled;
//    if (shadowEnabled)
//    {
//      loader_text_shadow.sourceComponent = component_shadow;
//    }
//    else
//      loader_text_shadow.sourceComponent = undefined;
  }

}
