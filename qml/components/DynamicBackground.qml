import QtQuick 2.2
import QtQuick.Particles 2.0
import "../../qml/utils.js" as Utils

Item
{
  property alias backgroundGrid: gridBG;
  property bool flyingTitles: true;

  anchors.fill: parent

  ParticleSystem { id: sys1; running: flyingTitles; }
//  ParticleSystem { id: sys2; }
//  ImageParticle
//  {
//    system: sys2
//    source: "qrc:/images/plexus_x_white.png"
//  }

  Grid
  {
    id: gridBG;

    property real maxOpacity: 0.4;
    property real minOpacity: 0.2;
    property int titleWidth: 50;
    property int titleHeight: 50;

    anchors.fill: parent;

    columns: (parent.width / titleWidth);
    rows: (parent.height / titleHeight);

    spacing: 5

    Repeater
    {
      model: gridBG.rows * gridBG.columns;
//      model: ((parent.height / gridBG.rows) * (parent.width / gridBG.columns));
      Rectangle
      {
        opacity: gridBG.minOpacity
        width: gridBG.titleWidth;//Utils.getRandom(10, 50);
        height: gridBG.titleHeight;
        color: "transparent"

//        function swap()
//        {
//          rot.axis.x = (rot.axis.x = 0)? 1:0;
//          rot.axis.y = (rot.axis.y = 1)? 0:1;
//          //        console.log("swap")
//        }

        transform: [
          Rotation {
            id: rot;
            angle: 0;
            axis { x: 0; y: 1; z:0 }
            origin.x: 0
            origin.y: 0.5

            SequentialAnimation on angle {
              loops: Animation.Infinite
              PauseAnimation { duration: Utils.getRandom(7000, 20000); }
              NumberAnimation { from: 0; to: -90; duration: Utils.getRandom(500, 1000); }
              PauseAnimation { duration: Utils.getRandom(3000, 8000); }
              NumberAnimation { from: -90; to: 0; duration: Utils.getRandom(500, 1000); }
            }

          }
        ]

        SequentialAnimation on opacity {
          loops: Animation.Infinite
          PauseAnimation { duration: Utils.getRandom(200, 1000); }
          NumberAnimation { from: gridBG.minOpacity; to: gridBG.maxOpacity; duration: Utils.getRandom(500, 1000); }
          PauseAnimation { duration: Utils.getRandom(0, 1000); }
          NumberAnimation { from: gridBG.maxOpacity; to: gridBG.minOpacity; duration: Utils.getRandom(500, 1000); }
        }


        SequentialAnimation on color {
          loops: Animation.Infinite

          PauseAnimation { duration: Utils.getRandom(100, 4000); }
          ColorAnimation
          {
            from: "orange"
            to: "red"
            duration: Utils.getRandom(200, 5000);
          }
          PauseAnimation { duration: Utils.getRandom(100, 4000); }
          ColorAnimation
          {
            from: "red"
            to: "blue"
            duration: Utils.getRandom(200, 5000);
          }
          PauseAnimation { duration: Utils.getRandom(100, 4000); }
          ColorAnimation
          {
            from: "blue"
            to: "white"
            duration: Utils.getRandom(200, 5000);
          }
          PauseAnimation { duration: Utils.getRandom(100, 4000); }
          ColorAnimation
          {
            from: "white"
            to: "green"
            duration: Utils.getRandom(200, 5000);
          }
          PauseAnimation { duration: Utils.getRandom(100, 4000); }
          ColorAnimation
          {
            from: "green"
            to: "orange"
            duration: Utils.getRandom(200, 5000);
          }
          PauseAnimation { duration: Utils.getRandom(100, 4000); }

        }

//        Image
//        {
//          anchors.fill: parent
//          source: "qrc:/images/plexus_x_white.png"
//          fillMode: Image.PreserveAspectFit;
//        }


      }
    } // END REPEATER

  } // END GRID


  ItemParticle
  {
    system: sys1
    delegate: Rectangle {
      width: 50; height: 50;
      color: "black"
//      opacity: 0.5

//      SequentialAnimation on color {
//        loops: Animation.Infinite

//        ColorAnimation
//        {
//          from: "orange"
//          to: "red"
//          duration: 500
//        }
//        ColorAnimation
//        {
//          from: "red"
//          to: "blue"
//          duration: 500
//        }
//        ColorAnimation
//        {
//          from: "blue"
//          to: "white"
//          duration: 500
//        }
//        ColorAnimation
//        {
//          from: "white"
//          to: "green"
//          duration: 500
//        }
//        ColorAnimation
//        {
//          from: "green"
//          to: "orange"
//          duration: 500
//        }

//      }

      SequentialAnimation on color {
        loops: Animation.Infinite

//        PauseAnimation { duration: Utils.getRandom(100, 4000); }
        ColorAnimation
        {
          from: "orange"
          to: "red"
          duration: Utils.getRandom(200, 600);
        }
//        PauseAnimation { duration: Utils.getRandom(100, 4000); }
        ColorAnimation
        {
          from: "red"
          to: "blue"
          duration: Utils.getRandom(200, 600);
        }
//        PauseAnimation { duration: Utils.getRandom(100, 4000); }
        ColorAnimation
        {
          from: "blue"
          to: "white"
          duration: Utils.getRandom(200, 600);
        }
//        PauseAnimation { duration: Utils.getRandom(100, 4000); }
        ColorAnimation
        {
          from: "white"
          to: "green"
          duration: Utils.getRandom(200, 600);
        }
//        PauseAnimation { duration: Utils.getRandom(100, 4000); }
        ColorAnimation
        {
          from: "green"
          to: "orange"
          duration: Utils.getRandom(200, 600);
        }
//        PauseAnimation { duration: Utils.getRandom(100, 4000); }

      }

    }

  } // END ITEMPARTICLE


  Emitter
  {
    id: emitterTop
    enabled: flyingTitles; visible: flyingTitles;
    system: sys1
    emitRate: 10
    lifeSpan: 2000

    //    y: mouseArea.pressed ? mouseArea.mouseY : circle.cy
    //    x: mouseArea.pressed ? mouseArea.mouseX : circle.cx

    height: 1
    anchors.left: parent.left;
    anchors.right: parent.right;
    anchors.top: parent.top;


    velocity: PointDirection {xVariation: 0; yVariation: 1000;}
    acceleration: PointDirection {xVariation: 0; yVariation: 10;}
//    velocityFromMovement: 3

    size: 50//progress/4;
    sizeVariation: 12
  }

  Emitter
  {
    id: emitterBottom
    enabled: flyingTitles; visible: flyingTitles;
    system: sys1
    emitRate: 10
    lifeSpan: 2000

    //    y: mouseArea.pressed ? mouseArea.mouseY : circle.cy
    //    x: mouseArea.pressed ? mouseArea.mouseX : circle.cx

    height: 1
    anchors.left: parent.left;
    anchors.right: parent.right;
    anchors.bottom: parent.bottom;


    velocity: PointDirection {xVariation: 0; yVariation: -1000;}
    acceleration: PointDirection {xVariation: 0; yVariation: 10;}
//    velocityFromMovement: 3

    size: 50//progress/4;
    sizeVariation: 12
  }

  Emitter
  {
    id: emitterLeft
    enabled: flyingTitles; visible: flyingTitles;
    system: sys1
    emitRate: 10
    lifeSpan: 2000

    //    y: mouseArea.pressed ? mouseArea.mouseY : circle.cy
    //    x: mouseArea.pressed ? mouseArea.mouseX : circle.cx

    width: 1

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;

    anchors.left: parent.left;
//    anchors.right: parent.right;


    velocity: PointDirection {xVariation: 1000; yVariation: 0;}
    acceleration: PointDirection {xVariation: 10; yVariation: 0;}
//    velocityFromMovement: 3

    size: 50//progress/4;
    sizeVariation: 12
  }

  Emitter
  {
    id: emitterRight
    enabled: flyingTitles; visible: flyingTitles;
    system: sys1
    emitRate: 10
    lifeSpan: 2000

    //    y: mouseArea.pressed ? mouseArea.mouseY : circle.cy
    //    x: mouseArea.pressed ? mouseArea.mouseX : circle.cx

    width: 1

    anchors.top: parent.top;
    anchors.bottom: parent.bottom;

//    anchors.left: parent.left;
    anchors.right: parent.right;


    velocity: PointDirection {xVariation: -1000; yVariation: 0;}
    acceleration: PointDirection {xVariation: -10; yVariation: 0;}
//    velocityFromMovement: 3

    size: 50//progress/4;
    sizeVariation: 12
  }

}
