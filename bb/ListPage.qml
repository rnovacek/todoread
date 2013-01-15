
import bb.cascades 1.0

Page {
    id: page

    property variant selected: null
    property bool showRead: false
    signal openUrl(variant item)

    actions: [
        ActionItem {
            title: "Refresh"
            imageSource: "reload_dark.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                controller.reloadModel();
            }
        },
        /*
        ActionItem {
            title: "Download All"
            imageSource: "/usr/share/icons/bb_action_archive.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {

            }
        },
        */
        ActionItem {
            title: "About"
            imageSource: "info_dark.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: rootWindow.push(aboutPage)
        },
        ActionItem {
            title: "Help!"
            imageSource: "help_dark.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                controller.openExternal("https://github.com/rnovacek/todoread/wiki/Getting-Started")
            }
        },
        ActionItem {
            title: "Configuration"
            imageSource: "settings_dark.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: rootWindow.push(configPage)
        },
        ActionItem {
            title: bbReadModel.showRead ? "Hide read articles" : "Show read articles"
            imageSource: "eye_dark.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                page.showRead = !page.showRead
            }
        }
    ]

    ListView {
        id: tableView
        dataModel: bbReadModel
        property alias showRead: page.showRead
        // Workaround to be able to access context property in the item
        function openExternal(url) {
            controller.openExternal(url);
        }

        listItemComponents: ListItemComponent {
            type: "item"
            StandardListItem {
                id: item
                //imageSource: rootWindow.showFavicon ? "" : (controller.networkOnline ? "http://www.google.com/s2/favicons?domain=" + model.origUrl.match(/:\/\/(.[^/]+)/)[1]  : "")
                title: ListItemData.title
                status: ListItemData.status == 0 ? "" : (ListItemData.status == 1 ? "read" : "deleted")
                description: ListItemData.url
                visible: (ListItemData.status == 0) || (ListItem.view.showRead && ListItemData.status == 1)
                contextActions: [
                    ActionSet {
                        ActionItem {
                            title: "Open in external browser"
                            imageSource: "browser_dark.png"
                            onTriggered: {
                                // Workaround - can't access context property from here
                                item.ListItem.view.openExternal(ListItemData.url)
                            }
                        }
                        ActionItem {
                            title: ListItemData.status == 0 ? "Mark as read" : "Unmark as read"
                            imageSource: ListItemData.status == 0 ? "unread_dark.png" : "read_dark.png"
                            onTriggered: {
                                if (ListItemData.status == 0) {
                                    ListItemData.markAsRead();
                                } else {
                                    ListItemData.unmarkAsRead();
                                }
                            }
                        }
                    }
                ]
            }
        }
        onTriggered: {
            openUrl(bbReadModel.data(indexPath));
        }
    }
}
