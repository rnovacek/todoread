import QtQuick 1.1

import com.nokia.meego 1.0

import "ReadModel.js" as Model

PageStackWindow {
    id: rootWindow

//    width: 480
//    height: 480

    property int currentIndex: -1
    property variant currentItem: null

    property bool showFavicons: true
    property bool useExternalBrowser: false
    property bool markAutomatically: false

    initialPage: ListPage {
        id: list
        onOpenUrl: {
            openBrowser(index, item)
        }
    }

    function openBrowser(index, item) {
        console.log("openBrowser: " + item.url)
        // Open item in internal or external browser
        if (controller.configValue("useExternalBrowser", false)) {
            controller.openExternal(item.url)
        } else {
            currentItem = item
            currentIndex = index
            browserPage.url = item.url
            browserPage.isRead = item.isRead
            pageStack.push(browserPage)
        }

        // Mark item as read if it's configured that way
        if (controller.configValue("markAutomatically", false)) {
            Model.setValue(item, "isRead", !item.isRead)
        }
    }

    function showError(message) {
        controller.error(message);
    }

    Component.onCompleted: {
        reloadConfig();
    }

    function reloadConfig() {
        showFavicons = controller.configValue("showFavicons", true)
        useExternalBrowser = controller.configValue("useExternalBrowser", false)
        markAutomatically = controller.configValue("markAutomatically", false)
    }

    Component.onDestruction: {
        var array = new Array();
        var i, j;
        for (i in Model.items) {
            var item = Model.items[i];
            var subArray = new Array();
            for (j in item) {
                switch (typeof(item[j])) {
                case "string":
                    subArray.push('"' + j + '":"' + item[j] + '"');
                    break;
                case "number":
                    subArray.push('"' + j + '":' + item[j]);
                    break;
                case "boolean":
                    subArray.push('"' + j + '":' + (item[j] ? "true" : "false"));
                    break;
                }
            }
            array.push("{" + subArray.join(",") + "}");
        }
        var dump = "[" + array.join(",") + "]";
        controller.save(dump);
    }

    AboutPage {
        id: aboutPage
    }

    BrowserPage {
        id: browserPage

        onMarkAsRead: {
            Model.setValue(currentItem, "isRead", !currentItem.isRead)
        }
    }

    ConfigPage {
        id: configPage
    }

    Connections {
        target: controller;
        onItemDownloadFinished: {
            console.log("itemDownloadFinished " + item + " " + item.data);
            for (var i in item)
                console.log(i + ": " + item[i]);
            Model.setValue(item, "isDownloaded", true)
            Model.setValue(item, "isDownloading", false)
            Model.setValue(item, "url", newUrl)
        }
    }
}
