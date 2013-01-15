
import bb.cascades 1.0

NavigationPane {
    id: rootWindow

    property int currentIndex: -1
    property variant currentItem: null

    property bool showFavicons: true
    property bool useExternalBrowser: false
    property bool markAutomatically: false

    ListPage {
        id: list
        onOpenUrl: {
            openBrowser(item)
        }
    }

    function openBrowser(item) {
        console.log("openBrowser: " + item.url);
        // Open item in internal or external browser
        if (controller.configValue("useExternalBrowser", false)) {
            controller.openExternal(item.url);
        } else {
            browserPage.setItem(item);
            rootWindow.push(browserPage);
        }

        // Mark item as read if it's configured that way
        if (controller.configValue("markAutomatically", false)) {
            item.markAsRead();
        }
    }

    /*
    function showError(message) {
        controller.error(message);
    }

    function reloadConfig() {
        showFavicons = controller.configValue("showFavicons", true)
        useExternalBrowser = controller.configValue("useExternalBrowser", false)
        markAutomatically = controller.configValue("markAutomatically", false)
    }
    */
    attachedObjects: [
        AboutPage {
            id: aboutPage
        },
        BrowserPage {
            id: browserPage
        },
        ConfigPage {
            id: configPage
        }
        /*
        Dialog {
            id: errorDialog
            property string titleText: "Error"
            property string message: "Error"
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                Label {
                    text: errorDialog.message
                }

                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "OK"
                    onClicked: errorDialog.close()
                }
            }
            titleText: "Error"
            message: "Error"
        }
        */
    ]
}
