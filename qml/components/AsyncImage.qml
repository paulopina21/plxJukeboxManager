import QtQuick 2.0

Image
{
  id: imgContent
  asynchronous: true
  opacity: 0

  property string fallback
  property int animDuration: 500

  fillMode: Image.PreserveAspectFit

  Behavior on opacity {
    PropertyAnimation { duration: animDuration }
  }

  onStatusChanged:
  {
    switch (status)
    {
      case Image.Ready: opacity = 1
      break;
      case Image.Error: source = fallback
      break;
    }
  }


}
