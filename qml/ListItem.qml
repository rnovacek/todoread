
import QtQuick 1.1

import com.nokia.meego 1.0

import "ReadModel.js" as Model

Rectangle {
    property string label: ""
    property string url

    signal openUrl(variant item)
    signal markAsRead(variant item)
    signal downloadRequest(variant item)

    color: (index % 2 == 0) ? "#dddddd" : "white"
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

    Image {
        id: favicon
        asynchronous: true
        source: "http://www.google.com/s2/favicons?domain=" + model.origUrl.match(/:\/\/(.[^/]+)/)[1]
        width: 32
        height: 32
        anchors {
            top: parent.top
            topMargin: parent.height / 2 - 16
            left: downloadMark.right
            leftMargin: 8
        }
    }

    Text {
        id: label
        elide: Text.ElideRight
        text: title
        font.bold: !model.isRead
        font.pixelSize: 24
        anchors {
            left: favicon.right
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
        font.pixelSize: 24
        anchors {
            left: favicon.right
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
                    Model.setValue(model, "isRead", !model.isRead)
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
