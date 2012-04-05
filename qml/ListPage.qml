import QtQuick 1.1

import com.nokia.meego 1.0

import "ReadModel.js" as Model

Page {
    id: page

    property variant selected: null
    property alias model: tableView.model

    signal openUrl(int index, variant item)

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-refresh"
            onClicked: model.sync()
        }
        ToolIcon {
            iconId: "toolbar-directory-move-to"
            onClicked: {
                var items = new Array();
                for (var i = 0; i < tableView.model.count; i++) {
                    items.push(tableView.model.get(i));
                }
                controller.downloadItems(items);
            }
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            anchors.right: parent.right
            onClicked: mainMenu.open();
        }
    }

    Menu {
        id: mainMenu
        MenuLayout {
            MenuItem {
                text: "Configuration"
                onClicked: pageStack.push(configPage)
            }
            MenuItem {
                text: model.showRead ? "Hide read" : "Show read"
                onClicked: {
                    model.showRead = !model.showRead
                }
            }
        }
    }

    ListView {
        id: tableView
        anchors.fill: parent
        anchors.topMargin: 16
        model: ReadModel {
            id: model
            onShowReadChanged: {
                reload();
                controller.setConfigValue("showRead", showRead);
            }
        }

        delegate: ListItem {
            onOpenUrl: {
                console.log("openUrl handler")
                page.openUrl(index, model)
            }
        }

        Component.onCompleted: {
            var dump = controller.load();
            Model.load(dump);
            model.reload();
        }
    }
    ScrollDecorator {
        flickableItem: tableView
    }
}
