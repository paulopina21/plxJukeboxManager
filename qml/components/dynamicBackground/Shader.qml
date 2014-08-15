import QtQuick 2.2

ShaderEffect
{
  id: effectShader;
  anchors.fill: parent;

  blending: false;
  cullMode: ShaderEffect.BackFaceCulling;

  property variant source;
  property string fragmentCode;

  property ListModel parameters: ListModel { }
  property int iGlobalTimePrecision: 1;
  property real iGlobalTimeIncrement: 0.05;


  property var iChannel0;
  property var iChannel1;
  property var iChannel2;
  property var iChannel3;

  property real iGlobalTime: 0.0;
  property vector3d iResolution: Qt.vector3d(width,height,0);


  Timer
  {
    running: true;
    repeat: true;
    interval: effectShader.iGlobalTimePrecision;
    onTriggered: effectShader.iGlobalTime += effectShader.iGlobalTimeIncrement;
  }

  onIChannel0Changed: fragmentShader = "uniform sampler2D iChannel0;" + fragmentShader;
  onIChannel1Changed: fragmentShader = "uniform sampler2D iChannel1;" + fragmentShader;
  onIChannel2Changed: fragmentShader = "uniform sampler2D iChannel2;" + fragmentShader;
  onIChannel3Changed: fragmentShader = "uniform sampler2D iChannel3;" + fragmentShader;

  onFragmentCodeChanged:
  {
    fragmentShader +=
        "  uniform float iGlobalTime;
           uniform vec3 iResolution;
        "
    fragmentShader += fragmentCode;
  }
}
