import QtQuick 2.0
import Sailfish.Silica 1.0
import FilesModel 1.0
Page {
    id: firstPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Results")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height
        //// FilesModel {
        ////     id: fmodel
        //onReadyRead: firstLabel.text = readAll()
        //// }
        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: firstPage.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("gmLocate")
            }
            Label {
                id: iddf
                anchors.horizontalCenter: parent.horizontalCenter
                //x: Theme.horizontalPageMargin
                ////text: fmodel.diskFree()//qsTr("What do ya mis, alder?")
                text: sharedfmodel.diskFree()
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
                padding: Theme.paddingSmall

            }
            Label {
                id: idDBSystemAge
                anchors.horizontalCenter: parent.horizontalCenter
                //x: Theme.horizontalPageMargin
                ////text: fmodel.updateDb()
                text: sharedfmodel.updateDb(false)
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
            Label {
                id: idDBUserAge
                anchors.horizontalCenter: parent.horizontalCenter
                //x: Theme.horizontalPageMargin
                ////text: fmodel.updateDb()
                text: sharedfmodel.updateDb(true)
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
                //margins: Theme.paddingLarge // wenns an kanten klebt
            }

            ButtonLayout {
                id: idButtons
                Button {
                    text: "update User DB"
                    //anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        ///var/cache/locate/locatedb
                        //process.start("/bin/cat", [ "/proc/uptime" ]);
                        //process.start("df", [ "-h" ]);
                        idDBUserAge.text = sharedfmodel.updateDb(true, true)
                    }
                }
                /*Button {
                    text: "locate (User DB)"
                    ButtonLayout.newLine: true
                    enabled: file2find.text.length > 2 ? true : false
                    //   anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        sharedfmodel.ulocate(file2find.text, ignoreCaseSwitch.checked)
                        pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
                    }
                }*/

                Button {
                    text: " locate "
                    ButtonLayout.newLine: true
                    enabled: file2find.text.length > 2 ? true : false
                    //   anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        sharedfmodel.locate(file2find.text, useUserDBSwitch.checked, ignoreCaseSwitch.checked)
                        pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
                    }
                }
            }
            TextSwitch {
                id: useUserDBSwitch
                checked: false
                text: "use User DB"
                description: useUserDBSwitch.checked ? "using User DB" : "using System DB"
            }
            TextSwitch {
                id: ignoreCaseSwitch
                checked: true
                text: "ignore case"
                description: ignoreCaseSwitch.checked ? "ignoring case" : "case sensitive"
            }
            TextField {
                id: file2find
                placeholderText: "fillin file to find..."
                label: "fillin file to find... (min. 3 chars!)"
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                EnterKey.enabled: text.length > 2
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: {
                    sharedfmodel.locate(file2find.text , ignoreCaseSwitch.checked)
                    pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
                }
            }
        }
    }
}
