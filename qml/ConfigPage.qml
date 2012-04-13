import QtQuick 1.1

import com.nokia.meego 1.0

Page {
    anchors.margins: 16
    Column {
        anchors.fill: parent
        spacing: 5
        Item {
            width: parent.width
            height: showFavicons.height
            Label {
                text: "Show favicons"
                anchors.left: parent.left
            }
            Switch {
                id: showFavicons;
                anchors.right: parent.right
                onCheckedChanged: controller.setConfigValue("showFavicons", checked);
                Component.onCompleted: {
                    checked = controller.configValue("showFavicons", true);
                }
            }
        }
        Item {
            width: parent.width
            height: useExternalBrowser.height
            Label {
                text: "Open articles in external browser"
                anchors.left: parent.left
            }
            Switch {
                id: useExternalBrowser
                anchors.right: parent.right
                onCheckedChanged: controller.setConfigValue("useExternalBrowser", checked);
                Component.onCompleted: {
                    checked = controller.configValue("useExternalBrowser", false);
                }
            }
        }
        Item {
            width: parent.width
            height: markAutomatically.height
            Label {
                text: "Mark article as read when showed"
                anchors.left: parent.left
            }
            Switch {
                id: markAutomatically
                anchors.right: parent.right
                onCheckedChanged: controller.setConfigValue("markAutomatically", checked);
                Component.onCompleted: {
                    checked = controller.configValue("markAutomatically", false);
                }
            }
        }
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                rootWindow.reloadConfig();
                pageStack.pop();
            }
        }
    }
}
