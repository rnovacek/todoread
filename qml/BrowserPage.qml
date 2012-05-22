
import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.meego 1.0

Page {

    property string url: ""

    onUrlChanged: browser.url = url

    signal markAsRead

    Browser {
        id: browser
        anchors.fill: parent
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
        ToolIcon {
            iconId: "toolbar-done"
            onClicked: {
                markAsRead();
            }
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            anchors.right: parent == undefined ? undefined : parent.right
            onClicked: browserMenu.open();
        }
    }

    Menu {
        id: browserMenu
        MenuLayout {
            MenuItem {
                text: "Open in external browser"
                onClicked: controller.openExternal(browser.url)
            }
        }
    }
}