import QtQuick 1.1

import "ReadModel.js" as Model

ListModel {
    id: model

    property bool showRead: false

    signal reloaded;

    function init() {
        showRead = controller.configValue("showRead", false);
        Model.controller = controller;
    }

    function createModel(text) {
        console.log("createModel: " + text);
        Model.items = new Array();
        var data = eval('(' + text + ')');
        for (var id in data.list) {
            var item = data.list[id];
            var it = new Model.ReadItem(item['item_id'], item['title'], item['url'], item['state']);
            Model.items.push(it);
        }
    }

    function reload() {
        model.clear();
        for (var id in Model.items) {
            if (showRead || !Model.items[id].isRead) {
                Model.items[id].model = model;
                Model.items[id].index = model.count;
                model.append(Model.items[id]);
                model.set(model.count - 1, {'data': Model.items[id]});
            } else {
                Model.items[id].model = null;
                Model.items[id].index = -1;
            }
        }
        reloaded();
    }

    function sync() {
        sendRead(true);
        // Download will be called automatically from onreadystatechanged
    }

    function handleResult(status) {
        console.log("handleResult " + status)
        switch (status) {
        case 0:
            return true;
        case 200:
            return true;
        case 400:
            rootWindow.showError("Request to server is not valid. File a bug, please.");
            return false;
        case 401:
            rootWindow.showError("Username/password incorrect.");
            return false;
        case 403:
            rootWindow.showError("Rate limit exceeded, please wait a little bit before resubmitting");
            return false;
        case 503:
            rootWindow.showError("Read It Later's sync server is down for scheduled maintenance.");
            return false;
        }
        rootWindow.showError("Unknown error code: " + status);
        return false;
    }

    function sendRead(doDownload) {
        var readItems = new Array(), unreadItems = new Array();
        var readIndex = 0, unreadIndex = 0;
        for (var id in Model.items) {
            var item = Model.items[id];
            if (item.isRead != item.isOrigRead) {
                if (item.isRead) {
                    console.log("Marked as read: " + id + "\t" + item);
                    // Mark item as read
                    readItems.push('"' + readIndex + '":{"url":"' + item.origUrl + '"}');
                    readIndex++;
                } else {
                    console.log("Unmarked as read: " + id + "\t" + item);
                    unreadItems.push('"' + unreadIndex + '":{"title":"' + item.title + '","url":"' + item.origUrl + '"}');
                    unreadIndex++;
                }
            }
        }
        if (readItems.length > 0 || unreadItems.length > 0) {
            // Send read/unread items

            var request = new XMLHttpRequest();
            request.onreadystatechange = function() {
                if (request.readyState === XMLHttpRequest.DONE) {
                    if (handleResult(request.status)) {
                        if (doDownload) {
                            download();
                        }
                    }
                }
            }

            request.open("POST", "https://readitlaterlist.com/v2/send", true);
            var username = controller.configValue("username", "");
            var password = controller.configValue("password", "");
            var apikey = "926TnD0Vg5663lb9e7dc565vF9p4cW5a";

            console.log("Logging in using " + username + " and " + password);
            console.log("Read items dump: " + data);

            request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            var params = "username=" + encodeURIComponent(username) +
                         "&password=" + encodeURIComponent(password) +
                         "&apikey=" + encodeURIComponent(apikey)
            if (readItems.length > 0) {
                params += "&read=" + encodeURIComponent("{" + readItems.join(",") + "}");
            }
            if (unreadItems.length > 0) {
                params += "&new=" + encodeURIComponent("{" + unreadItems.join(",") + "}");
            }

            request.send(params);
        } else {
            if (doDownload) {
                download();
            }
        }
    }

    function download() {
        var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
            if (request.readyState === XMLHttpRequest.DONE) {
                if (handleResult(request.status)) {
                    createModel(request.responseText);
                    reload();
                }
            }
        }

        request.onerror = function(error) {
             console.log("error: " + error);
         }

        request.open("POST", "https://readitlaterlist.com/v2/get", true);
        var username = controller.configValue("username", "");
        var password = controller.configValue("password", "");
        var apikey = "926TnD0Vg5663lb9e7dc565vF9p4cW5a";

        console.log("Logging in using " + username + " and " + password);

        // TODO: Limit it by using timestamp
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        request.send("username=" + encodeURIComponent(username) +
                     "&password=" + encodeURIComponent(password) +
                     "&apikey=" + encodeURIComponent(apikey));
    }

    function downloadItem(item) {
        item.model.set(item.count, {"isDownloading": true});
        controller.downloadItem(item);
    }
}
