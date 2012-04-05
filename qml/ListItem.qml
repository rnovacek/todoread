
import QtQuick 1.1

import com.nokia.meego 1.0

Rectangle {
    property string label: ""
    property string url

    signal openUrl(variant item)
    signal markAsRead(variant item)
    signal downloadRequest(variant item)

    color: (index % 2 == 0) ? "white" : "gray"
    width: parent.width
    height: label.height * 2 + 10

    Rectangle {
        id: downloadMark
        color: "green"
        height: parent.height - 6
        width: 3
        visible: model.isDownloaded
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 5
            topMargin: 3
        }
    }

    Text {
        id: label
        elide: Text.ElideRight
        text: title
        font.bold: !model.isRead
        anchors {
            left: downloadMark.right
            right: parent.right
            top: parent.top
            leftMargin: 8
            rightMargin: 8
            topMargin: 5
        }
    }

    Text {
        id: url
        elide: Text.ElideRight
        text: origUrl
        color: "gray"
        anchors {
            left: downloadMark.right
            right: parent.right
            top: label.bottom
            leftMargin: 8
            rightMargin: 8
        }
    }


    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Opening url " + url)
            openUrl(model.modelData)
        }
        onPressAndHold: myContextMenu.open()
    }

    ContextMenu {
        id: myContextMenu
        MenuLayout {
            MenuItem {
                text: model.isRead ? "Unmark as read" : "Mark as read"
                onClicked: {
                    tableView.model.set(index, {"isRead": !model.isRead});
                }
            }
            MenuItem {
                text: "Download"
                visible: !isDownloaded
                onClicked: {
                    tableView.model.downloadItem(model);
                }
            }
        }
    }
}
