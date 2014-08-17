import QtQuick 2.2
//import QtQuick.Particles 2.0
import QtGraphicalEffects 1.0

import "utils.js" as Utils
import "components"
import "components/dynamicBackground"

Rectangle
{
  id: root
  height: screenGeometry.height
  width: screenGeometry.width
  property int progress: 0

  color: "black"

  Timer
  {
    id: timer_Loading
    interval: 1000; running: true; repeat: true;
    onTriggered:{
      interval = 100
      root.progress = application.progress;
      if (application.progress == 100)
      {
        if (application.isCatalogSelected())
          loader_main.source = "qrc:/qml/albumbuilder/MainView.qml";
        else{
          loader_main.source = "qrc:/qml/ViewEmpty.qml";
        }
      }
    }
  }

  //  Image
  //  {
  //    anchors.fill: parent
  //    source: "qrc:/images/loading.jpg";
  //  }

  //  MouseArea
  //  {
  //    id: mouseArea
  //    anchors.fill: root
  //  }

//  Connections
//  {
//    target: application
//    onProgressChanged:
//    {
//      root.progress = progress;
//      console.log('PROGRESS = '+progress);
//    }
//  }

  Component.onCompleted:
  {
    var randr = Utils.getRandom(1,9);
    var folder = "qrc:/qml/components/dynamicBackground/";
//      if (valentinesDay)
//      {
//        source = folder + "Heart.qml"; return;
//      }

    switch (randr)
    {
      // LIGHT ONES
      case 1: loaderBackground.source = folder + "Waves.qml"; break;
      case 2: loaderBackground.source = folder + "Simplicity.qml"; break;
      case 3: loaderBackground.source = folder + "CirclePattern.qml"; break;
      case 4: loaderBackground.source = folder + "Blobs.qml"; break;
      case 5: loaderBackground.source = folder + "FlappingWings.qml"; break;
      case 6: loaderBackground.source = folder + "A704.qml"; break;
      case 7: loaderBackground.source = folder + "LoadingOrb.qml"; break;
      case 8: loaderBackground.source = folder + "SpaceEye.qml"; break;
      case 9: loaderBackground.source = folder + "NyanCat.qml"; break;

      // MEDIUM ONES -> NOT RECOMMENDED FOR PRODUCTION USE
      case 10: loaderBackground.source = folder + "Bubbles.qml"; break;
      case 11: loaderBackground.source = folder + "Tunnel1.qml"; break;
      case 12: loaderBackground.source = folder + "TruchetTentacles.qml"; break;
      case 13: loaderBackground.source = folder + "Industry.qml"; break;
      case 14: loaderBackground.source = folder + "ALotOfSpheres.qml"; break;
      case 15: loaderBackground.source = folder + "Pangram.qml"; break;

      // HEAVY ONES -> NO WAY TO USE IN THE WILD
      case 16: loaderBackground.source = folder + "DataTransfer.qml"; break;

//        case 99: loaderBackground.source = folder + "IO.qml"; break;
//        case 99: loaderBackground.source = folder + "TalisTraps.qml"; break;
//        case 99: loaderBackground.source = folder + "HotShower.qml"; break;
    }
    console.log("Selected: "+randr + "\nsource: "+loaderBackground.source);
  }



  //  ShadowText
  //  {
  //    id: text_progress
  //    //    x: circle.x
  //    y: circle.y - 23
  //    text: root.progress + "%"
  //    pixelSize: 50
  //    visible: false;

  //    textHAlignment: Text.AlignHCenter
  //    anchors.horizontalCenter: parent.horizontalCenter
  //    anchors.horizontalCenterOffset: 8
  //    shadowEnabled: false;
  //  }

//  DynamicBackground
//  {
//    id: bg
//    anchors.fill: parent;
//  }

//  FastBlur
//  {
//    visible: true
//    anchors.fill: bg
//    source: bg
//    radius: 64
//  }
//  ShaderEffectSource
//  {
//    id: theSource
//    smooth: false
//    hideSource: false
//    sourceItem: root;
//  }

//  Image
//  {
//    id: render
//    anchors.fill: parent;
//  }

  Loader
  {
    id: loaderBackground;
    anchors.fill: parent;
  }

//  Waves{}
//  Simplicity{}
//  CirclePattern{}
//  Blobs{}
//  FlappingWings{}
//  Bubbles{}
//  LoadingOrb{}
//  SpaceEye{}
//  NyanCat{}
//  A704{}

//  Tunnel1{}
//  ALotOfSpheres{}
//  TruchetTentacles{}
//  Industry{}
//  Pangram{}

//  DataTransfer{}

//  IO{} // Not working... need music
//  TalisTraps{} // Not Working
//  HotShower{} // Not working... Need NSA computers to render =)

//  Heart{} // SPECIAL FOR VALENTINE'S DAY

//  ShaderEffect
//  {
//    id: effectLasers;
//    anchors.fill: parent;
//    //property variant source: render;
//    property ListModel parameters: ListModel { }
//    property real iGlobalTime: 0.0
//    property vector3d iResolution: Qt.vector3d(width,height,0);

//    Timer
//    {
//        running: true;
//        repeat: true;
//        interval: 2;
//        onTriggered: effectLasers.iGlobalTime += 0.05;
//    }


//    fragmentShader: "
//      //varying mediump vec2 qt_TexCoord0;
//      uniform float iGlobalTime;
//      uniform vec3 iResolution;

//      //comment next line to disable DOF
//      #define DOF


//      //Rotation function by Syntopia
//      mat3 rotmat(vec3 v, float angle)
//      {
//        float c = cos(angle);
//        float s = sin(angle);

//        return mat3(c + (1.0 - c) * v.x * v.x, (1.0 - c) * v.x * v.y - s * v.z, (1.0 - c) * v.x * v.z + s * v.y,
//          (1.0 - c) * v.x * v.y + s * v.z, c + (1.0 - c) * v.y * v.y, (1.0 - c) * v.y * v.z - s * v.x,
//          (1.0 - c) * v.x * v.z - s * v.y, (1.0 - c) * v.y * v.z + s * v.x, c + (1.0 - c) * v.z * v.z
//          );
//      }

//      //Distance Field
//      vec4 de(vec3 pos) {
//        vec3 A=vec3(4.);
//        vec3 p = abs(A-mod(pos,2.0*A)); //tiling fold by Syntopia
//        float sph=length(p)-.6;
//        float cyl=length(p.xy)-.012;
//        cyl=min(cyl,length(p.xz))-.012;
//        cyl=min(cyl,length(p.yz))-.012;
//        p=p*rotmat(normalize(vec3(0,0,1.)),radians(45.));
//        if (max(abs(pos.x),abs(pos.y))>A.x) {
//        cyl=min(cyl,length(p.xy))-.012;
//        cyl=min(cyl,length(p.xz))-.012;
//        cyl=min(cyl,length(p.yz))-.012;
//        }
//         float d=min(cyl,sph);
//        vec3 col=vec3(0.);
//        if (sph<cyl && d<.1) col=vec3(.9,.85,.7); else col=vec3(1.2,0.2,0.1);
//        return vec4(col,d);

//      }


//      void main(void)
//      {
//        float time = iGlobalTime; //just because it's more handy :)

//        // mouse functions
////        vec2 mouse=iMouse.xy/iResolution.xy;
//        vec2 mouse=iResolution.xy;
////        float viewangle=-45.+iMouse.x/iResolution.x*90.;
//        float viewangle=-45.+iResolution.x*90.;
////        float focus=iMouse.y/iResolution.y*.4;
//        float focus=iResolution.y*.4;
////        if (mouse.x+mouse.y<.01) { //default settings
//          focus=.13;
//          viewangle=0.;
////        }

//        //camera
//        mat3 rotview=rotmat(vec3(0.,1.,0.),radians(viewangle));
//        vec2 coord = gl_FragCoord.xy / iResolution.xy *2.2 - vec2(1.);
//        coord.y *= iResolution.y / iResolution.x;
//        float fov=min((time*.2+.2),0.9); //animate fov at start
//        vec3 from = vec3(0.,sin(time*.5)*2.,time*5.);

//        vec3 p;
//        float totdist=-1.5;
//        float intens=1.;
//        float maxdist=90.;
//        vec3 col=vec3(0.);
//        vec3 dir;
//        for (int r=0; r<150; r++) {
//          dir=normalize(vec3(coord.xy*fov,1.))*rotview
//            *rotmat(normalize(vec3(0.05,0.05,1.)),time*.3+totdist*.015); //rotate ray
//          vec4 d=de(p); //get de and color
//          float distfactor=totdist/maxdist;
//          float fade=exp(-.06*distfactor*distfactor); //distance fade
//          float dof=min(.15,1.-exp(-2.*pow(abs(distfactor-focus),2.))); //focus
//          float dd=abs(d.w);
//          #ifdef DOF
//            totdist+=max(0.007+dof,dd); //bigger steps = out of focus
//          #else
//            totdist+=max(0.007,dd);
//          #endif
//          if (totdist>maxdist) break;
//          p=from+totdist*dir;
//          intens*=fade; //lower bright with distance
//          col+=d.xyz*intens; //accumulate color
//        }

//        col=col/maxdist; //average colors (kind of)
//        col*=pow(length(col),1.3)*.5; //contrast & brightness

//        //light
//        col+=vec3(1.1,.95,.85)*pow(max(0.,dot(dir,vec3(0.,0.,1.))),12.)*.8;
//        col+=vec3(.2,.17,.12)*pow(max(0.,dot(dir,vec3(0.,0.,1.))),200.);

//        col*=min(1.,time); //fade in

//        gl_FragColor = vec4(col,1.0);
//      }
//    "
//  }


  //  Item
  //  {
  //    id: circle
  //    x: root.width / 2.05
  //    y: root.height / 1.8
  ////    transform: Rotation { origin.x: 300; origin.y: 150; angle: 90}

  //    property real radiusX: 590
  //    property real radiusY: 260
  ////    property real dx: root.width / 2
  ////    property real dy: image_logo.y + image_logo.height + radius + 50
  //    property real cx: radiusX * Math.sin(percent*6.283185307179) + x
  //    property real cy: radiusY * Math.cos(percent*6.283185307179) + y - (cx/8)
  //    property real percent: 0

  //    SequentialAnimation on percent
  //    {
  //      loops: Animation.Infinite
  //      running: true

  //      NumberAnimation
  //      {
  //        duration: 1000
  //        from: 0
  //        to: 1
  //        loops: 8
  //      }

  //    }

  //  }

  Image
  {
    id: image_logo
    y: 100
    height: 100; width: parent.width;
    fillMode: Image.PreserveAspectFit
    source: "qrc:/images/plexus_white_big.png"
  }

  DropShadow
  {
    anchors.fill: image_logo
    source: image_logo;
    horizontalOffset: 3
    verticalOffset: 3
    radius: 2.0
    samples: 16
    color: "#FF000000"
  }

  ShadowText
  {
    id: text_loading
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 40
    anchors.right: parent.right
    anchors.rightMargin: 40

    text: qsTr("Loading...");

    shadowEnabled: false;

    SequentialAnimation on opacity
    {
      loops: Animation.Infinite;
      running: true;
      NumberAnimation
      {
        duration: 650;
        from: 0;
        to: 1;
      }
      NumberAnimation
      {
        duration: 650;
        from: 1;
        to: 0;
      }
    }
  }

  ShadowText
  {
    id: text_progress
    anchors.top: image_logo.bottom
    anchors.topMargin: 40
    anchors.right: image_logo.right
    anchors.left: image_logo.left

    pixelSize: 40

    text: qsTr(root.progress + "%");
    visible: (root.progress > 0 && root.progress < 100);

    shadowEnabled: true;
    horizontalAlignment: Text.AlignHCenter
  }
}
