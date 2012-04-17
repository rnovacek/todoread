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
            onClicked: {
                busyIndicator.running = true;
                model.sync();
            }
        }
        ToolIcon {
            iconId: "toolbar-directory-move-to"
            onClicked: {
                var items = new Array();
                for (var i = 0; i < tableView.model.count; i++) {
                    model.downloadItem(tableView.model.get(i));
                }
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
                text: "About"
                onClicked: pageStack.push(aboutPage)
            }

            MenuItem {
                text: "Help!"
                onClicked: {
                    controller.openExternal("https://github.com/rnovacek/todoread/wiki/Getting-Started")
                }
            }

            MenuItem {
                text: "Configuration"
                onClicked: pageStack.push(configPage)
            }

            MenuItem {
                text: model.showRead ? "Hide read articles" : "Show read articles"
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
            onReloaded: {
                busyIndicator.running = false;
            }
            onError: {
                busyIndicator.running = false;
                rootWindow.showError(message);
            }
        }

        delegate: ListItem {
            onOpenUrl: {
                console.log("openUrl handler")
                page.openUrl(index, model)
            }
        }

        Component.onCompleted: {
            model.init();
            var dump = controller.load();
            Model.load(dump);
            model.reload();
        }
    }
    ScrollDecorator {
        flickableItem: tableView
    }
    Rectangle {
        id: busyIndicator
        color: "#aeeeeeee"
        property bool running: false
        anchors.fill: parent
        visible: false;
        BusyIndicator {
            id: indicator
            anchors.centerIn: parent
            running: false;
            platformStyle: BusyIndicatorStyle { size: "large" }
        }
        onRunningChanged: {
            indicator.running = running;
            visible = running;
        }
    }
}
