import QtQuick 2.0
import Sailfish.Silica 1.0
import FilesModel 1.0
Page {
    id: secondPage
    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All
    SilicaListView {
        id: listView
        model: sharedfmodel.getFileList()
        /*
            FilesModel {
            id: fmodel
        }*/
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("located files: " ) + sharedfmodel.getsMyCounter()
        }
        delegate: ListItem {
            id: listEntry
            contentHeight:Theme.fontSizeSmall * 1.4
            Label {
                x: Theme.horizontalPageMargin
                // modelData ist qml definition
                text:  modelData  //qsTr("Item") + " " + index
                color: listEntry.highlighted ? Theme.highlightColor : Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                padding: Theme.paddingSmall

            }
            onClicked: {
                sharedfmodel.swap2top(index)
                console.log("Clicked " + index)
                //listView.model = 1;
                //update listView Ansatz mit BeginInsertRows unfug da performance inrelevat
                listView.model = sharedfmodel.getFileList()
                //listView.forceLayout()// funktioniert nicht
            }
            menu: ContextMenu {

                MenuItem {
                    // implicitHeight: Theme.fontSizeExtraSmall * 1.2
                    text: "move to top"

                    onClicked: {
                        sharedfmodel.swap2top(index)
                        listView.model = sharedfmodel.getFileList()
                    }
                }
                MenuItem {
                    //   implicitHeight: Theme.fontSizeExtraSmall * 1.2
                    text: "remove from list"

                    onClicked: {
                        listEntry.remorseAction("Deleting", function() {
                            sharedfmodel.remove(index)
                            listView.model = sharedfmodel.getFileList()
                        })
                    }
                }
            }
        }
    }
}
//VerticalScrollDecorator {}

//onStatusChanged: {
//   if (status === PageStatus.Activating) {
//playlistView.positionViewAtIndex(ctl.player.playbackStatus.id, ListView.Center)
//fmodel.locate(firstPage.file2find.text)
// }
/*else if (status === PageStatus.Active) {
            //            pageStack.pushAttached(Qt.resolvedUrl("CurrentSong.qml"));
            if (mCurrentSongPage == undefined) {
                var currentSongComponent = Qt.createComponent(
                            Qt.resolvedUrl("CurrentSong.qml"))
                mCurrentSongPage = currentSongComponent.createObject(mainWindow)
            }
            pageStack.pushAttached(mCurrentSongPage)
        }*/


