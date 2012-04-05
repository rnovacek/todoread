import QtQuick 1.1

import com.nokia.meego 1.0

import "ReadModel.js" as Model

PageStackWindow {
    id: rootWindow

    //platformStyle: PageStackWindowStyle { id: defaultStyle }

    property int currentIndex: -1
    property variant currentItem: null

    initialPage: ListPage {
        id: list
        onOpenUrl: {
            openBrowser(index, item)
        }
    }

    function showLogin() {
        console.log("showLogin")
        pageStack.push(configPage)
    }

    function openBrowser(index, item) {
        console.log("openBrowser: " + item.url)
        currentItem = item
        currentIndex = index
        browserPage.url = item.url
        pageStack.push(browserPage)
    }

    function showError(error) {
        console.log(error);
    }

    Component.onDestruction: {
        console.log("main::save");
        var array = new Array();
        var i, j;
        for (i in Model.items) {
            var item = Model.items[i];
            console.log("Item: " + item)
            var subArray = new Array();
            for (j in item) {
                console.log(j + "\t" + item[j]);
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

    BrowserPage {
        id: browserPage

        onMarkAsRead: {
            list.model.set(currentIndex, {"isRead": !currentItem.isRead});
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
            /*
            item.model.set(item.index, {
                                        "isDownloading": false,
                                        "url": newUrl});
            */
        }
    }
}
