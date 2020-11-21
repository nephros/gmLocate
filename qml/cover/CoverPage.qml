import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        id: label
        anchors.centerIn: parent
        text: qsTr("gmLocate")
    }
    Image {
      anchors.centerIn: parent
      source: "image://theme/icon-cover-search"
      width: parent.width / 2
      height: parent.height / 2
      fillMode: Image.PreserveAspectFit
      opacity: 0.2
    }
    CoverActionList {
        id: coverAction
        CoverAction {
            iconSource: "image://theme/icon-l-document"
        }
    }
}
