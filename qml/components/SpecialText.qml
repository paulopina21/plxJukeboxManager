import QtQuick 2.2
import QtGraphicalEffects 1.0

Item
{
  id: container_specialText
  width: text_text.contentWidth; height: text_text.contentHeight
  clip: false;
  state: "NORMAL";

  property string text: "TEXT"
  property string fontColor: "white"
  //  property string fontFamily: "qrc:/fonts/Oswald-Regular.ttf"//"Oswald"
  property string fontFamily: "Roboto [Condensed]"

  property int wrapMode: Text.NoWrap
  property int elide: Text.ElideNone

  property int pixelSize: config.scalerSize(22)
  property alias letterSpacing: text_text.font.letterSpacing
  property int capitalization: Font.AllUppercase
  property int textVAlignment: Text.AlignVCenter
  property int textHAlignment: Text.AlignLeft

  property bool bold: true
  property bool underline: false

  property bool asynchronous: true

  property bool editable: false

  property bool shadowEnabled: true

  property real shadowRadius: config.scalerSize(10)
  property real shadowSamples: 16
  property real shadowHOffset: 2
  property real shadowVOffset: 2
  property real shadowSpread: 0.3
  property bool shadowTransparentBorder: false;

  signal accept(string text);
  signal cancel(string text);
  signal clicked;

  states:
      [
    State
    {
      name: "NORMAL";
      PropertyChanges
      {
        target: loader_text_shadow;
        //        sourceComponent: (shadowEnabled)? component_shadow : undefined;
      }
    },
    State
    {
      name: "EDITING"
      PropertyChanges
      {
        target: loader_text_shadow
        sourceComponent: component_edit
        //activeFocus: true;
        focus: true;
      }
      PropertyChanges
      {
        target: text_text;
        visible: false;
      }
    }
  ]

  //  Component
  //  {
  //    id: component_shadow

  //    DropShadow
  //    {
  //      id: shadow_text
  //      //      source: text_text

  //      source: ShaderEffectSource {
  //        sourceItem: text_text
  //        //          filtering: ShaderEffectSource.Linear
  //      }
  //      //      blending: true


  //      anchors.fill: loader_text_shadow
  //      transparentBorder: container_specialText.shadowTransparentBorder
  //      radius: container_specialText.shadowRadius
  //      spread: container_specialText.shadowSpread
  //      samples: container_specialText.shadowSamples
  //      verticalOffset: container_specialText.shadowVOffset
  //      horizontalOffset: container_specialText.shadowHOffset
  //      color: "#80000000"
  //    }
  //  }

  Loader
  {
    id: loader_text_shadow
    focus: false
    asynchronous: container_specialText.asynchronous
    anchors.fill: text_text
  }

  ShadowText
  {
    id: text_text
    text: container_specialText.text
    color: container_specialText.fontColor
    //    visible: !shadowEnabled;

    shadowEnabled: true;

    anchors.fill: container_specialText

    verticalAlignment: container_specialText.textVAlignment
    horizontalAlignment: container_specialText.textHAlignment

    elide: container_specialText.elide
    wrapMode: container_specialText.wrapMode

    onTextChanged:
    {
      //      if (text === "")
      //      {
      //        loader_text_shadow.sourceComponent = undefined;
      //      }
      //      else if (container_specialText.shadowEnabled)
      //      {
      //        loader_text_shadow.sourceComponent = component_shadow;
      //        //        loader_text_shadow.item.source = text_text;
      //        //        loader_text_shadow.item.anchors.fill = loader_text_shadow;
      //      }
    }

    font.bold: container_specialText.bold
    font.family: container_specialText.fontFamily
    font.underline: container_specialText.underline
    font.pixelSize: container_specialText.pixelSize
    //    font.letterSpacing: container_specialText.letterSpacing
    font.capitalization: container_specialText.capitalization
  }



  //  ShaderEffectSource {
  //    id: effectSource
  //    sourceItem: text_text
  //    hideSource: true
  //    smooth: false
  //  }


  //  ShaderEffect
  //  {
  //    id: effectItem
  //    property variant texture: effectSource
  //    property real scaleValue: 1
  //    property bool distanceEnabled: false
  //    property color color: text_text.color

  //    width: text_text.width * scaleValue// *20
  //    height: text_text.height * scaleValue// *20
  //    anchors.centerIn: parent

  //    fragmentShader:
  //      "varying highp vec2 sampleCoord;
  //      uniform sampler2D texture;
  //      uniform lowp float qt_Opacity;
  //      uniform highp float scaleValue;
  //      uniform bool distanceEnabled;
  //      highp vec4 color;

  //      varying highp vec2 qt_TexCoord0;
  //      void main() {
  //          if (distanceEnabled) {
  ////              lowp vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
  //              lowp vec4 color2 = vec4(1.0, 1.0, 1.0, 1.0);
  //              lowp vec4 pix = texture2D(texture, qt_TexCoord0);

  //              pix = smoothstep(0.00001, 1.0, pix.a) * color;// * smoothstep(0.00001, 1.0, pix.r);
  //              gl_FragColor = vec4(pix.rgb, 1.0);
  //              // This is distance field version of the original image, with antialising
  //              //pix = color2 * smoothstep(0.35 + scaleValue * 0.12, 0.65 - scaleValue * 0.12, pix.r);
  //              //gl_FragColor = vec4(pix.rgb, 1.0);

  //              // This version is fast, but without antialiasing
  //              //pix = color2 * step(0.6, pix.r);
  //              //gl_FragColor = vec4(pix.rgb, 1.0);

  //              // This version gives transparency and outline effects
  ////              pix = color2 * smoothstep(0.50, 0.70 - scaleValue * 0.15, pix.r) + color * smoothstep(0.30 + scaleValue * 0.15, 0.50 , pix.r);
  ////              gl_FragColor = pix;

  //              // This version is for smiley image, coloring it dynamically
  ////              lowp vec4 color3 = vec4(0.95, 0.8, 0.4, 1.0);
  ////              pix = color * smoothstep(0.10, 0.30 - scaleValue * 0.18, pix.r) +
  ////                    color2 * smoothstep(0.40, 0.70 - scaleValue * 0.18, pix.g) +
  ////                    color3 * smoothstep(0.40, 0.70 - scaleValue * 0.18, pix.b);
  ////              gl_FragColor = pix;

  //      } else {
  //              gl_FragColor = qt_Opacity * texture2D(texture, qt_TexCoord0);
  //          }
  //      }"
  //  }



  MouseArea
  {
    visible: editable
    anchors.fill: parent
    onDoubleClicked:
    {
      if (editable)
        container_specialText.state = "EDITING";
    }
  }

  Component
  {
    id: component_edit
    Item
    {
      Rectangle
      {
        id: container_textEdit
        clip: true;
        color: "#80000000"
        border.color: "white"
        width: textEdit_editable.width
        height: textEdit_editable.height

        TextEdit
        {
          id: textEdit_editable
          color: container_specialText.fontColor
          selectedTextColor: "black"
          selectionColor: "white"

          anchors.verticalCenter: container_specialText.verticalCenter
          anchors.horizontalCenter: container_specialText.horizontalCenter

          verticalAlignment: container_specialText.textVAlignment
          horizontalAlignment: container_specialText.textRAlignment

          wrapMode: TextEdit.NoWrap
          selectByMouse: true

          text: container_specialText.text;

          //       onFocusChanged: text === initialText? text = "": text = container_edit.text;

          font.bold: container_specialText.bold
          font.family: container_specialText.fontFamily
          font.underline: container_specialText.underline
          font.pixelSize: container_specialText.pixelSize
          font.letterSpacing: container_specialText.letterSpacing
          font.capitalization: container_specialText.capitalization

          Component.onCompleted:
          {
            forceActiveFocus();
            selectAll();
          }

          onActiveFocusChanged:
          {
            if (!activeFocus)
              container_specialText.state = "NORMAL";
          }

          Keys.onReturnPressed:
          {
            container_specialText.text = textEdit_editable.text;
            accept(textEdit_editable.text);
            focus = false;
          }

          Keys.onEscapePressed:
          {
            cancel(textEdit_editable.text);
            focus = false;
          }
        }
      }

    }

  }

}
