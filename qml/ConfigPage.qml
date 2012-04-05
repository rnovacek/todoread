import QtQuick 1.1

import com.nokia.meego 1.0

Page {
    anchors.margins: 16
    Column {
        anchors.fill: parent
        Label {
            text: "Service:"
        }
        Button {
            text: serviceDialog.model.get(serviceDialog.selectedIndex).name
            onClicked: serviceDialog.open()
            anchors.left: parent.left
            anchors.right: parent.right
        }
        Label {
            text: "Username:"
        }
        TextField {
            id: username
            anchors.left: parent.left
            anchors.right: parent.right
        }
        Label {
            text: "Password:"
        }
        TextField {
            id: password
            echoMode: TextInput.Password
            anchors.left: parent.left
            anchors.right: parent.right
        }
        Button {
            text: "Connect"
            onClicked: {
                controller.login(username.text, password.text)
            }
        }
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
        /*
        ToolIcon {
            iconId: "toolbar-view-menu"
            anchors.right: parent == undefined ? undefined : parent.right
            onClicked: mainMenu.open();
        }
        */
    }

    // TODO: Replace it with correct model
    SelectionDialog {
        id: serviceDialog
        titleText: "Select Service"
        selectedIndex: 0

        model: ListModel {
            ListElement { name: "Read It Later" }
            ListElement { name: "Placeholder #1" }
            ListElement { name: "Placeholder #2" }
        }
    }
}