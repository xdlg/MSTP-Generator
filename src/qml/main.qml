import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
  width: 320
  height: 240
  visible: true
  title: qsTr("MSTP Generator")

  Connections {
    target: imageProvider
    function onImageReady() {
      image.refresh();
    }
  }

  Image {
    id: image
    anchors.fill: parent
    source: "image://imageProvider/image"
    // Necessary to reload the image after changing the source
    cache: false

    function refresh() {
      var oldSource = source
      source = ""
      source = oldSource
    }
  }

  MouseArea {
    anchors.fill: parent
    cursorShape: Qt.PointingHandCursor
    hoverEnabled: false
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onClicked: (mouse) => {
      if (mouse.button == Qt.LeftButton)
        imageProvider.startStopGeneration()
      else
        imageProvider.resetImage()
    }
  }
}
