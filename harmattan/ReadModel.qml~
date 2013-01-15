import QtQuick 1.1

import "ReadModel.js" as Model

ListModel {
    id: model

    property bool showRead: false

    signal reloaded;
    signal error(string message);

    function init() {
        showRead = controller.configValue("showRead", false);
        Model.controller = controller;
    }

    function createModel(text) {
        if (model.length == 0) {
            return;
        }

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
        switch (status) {
        case 0:
            return true;
        case 200:
            return true;
        case 400:
            error("Request to server is not valid. File a bug, please.");
            return false;
        case 401:
            error("Username/password incorrect.");
            return false;
        case 403:
            error("Rate limit exceeded, please wait a little bit before resubmitting");
            return false;
        case 503:
            error("Read It Later's sync server is down for scheduled maintenance.");
            return false;
        }
        error("Unknown error code: " + status);
        return false;
    }

    function sendRead(doDownload) {
        var readItems = new Array(), unreadItems = new Array();
        var readIndex = 0, unreadIndex = 0;
        for (var id in Model.items) {
            var item = Model.items[id];
            if (item.isRead !== item.isOrigRead) {
                if (item.isRead) {
                    // Mark item as read
                    readItems.push('"' + readIndex + '":{"url":"' +
                                   item.origUrl + '"}');
                    readIndex++;
                } else {
                    unreadItems.push('"' + unreadIndex + '":{"title":"' +
                                     encodeURIComponent(item.title) + '","url":"' +
                                     item.origUrl + '"}');
                    unreadIndex++;
                }
            }
        }
        if (readItems.length > 0 || unreadItems.length > 0) {
            // Send read/unread items

            var request = new XMLHttpRequest();
            request.timeout = 10000;
            request.ontimeout = function() {
                error("Unable to update the list of articles.");
            }

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
        request.timeout = 10000;
        request.ontimeout = function() {
            error("Unable to update the list of articles.");
        }
        request.onreadystatechange = function() {
            if (request.readyState === XMLHttpRequest.DONE) {
                if (handleResult(request.status)) {
                    if (request.responseText.length > 0) {
                        createModel(request.responseText);
                        reload();
                    } else {
                        error("Unable to download data");
                    }
                }
            }
        }

        request.onerror = function(err) {
             error(err);
         }

        request.open("POST", "https://readitlaterlist.com/v2/get", true);
        var username = controller.configValue("username", "");
        var password = controller.configValue("password", "");
        var apikey = "926TnD0Vg5663lb9e7dc565vF9p4cW5a";

        // TODO: Limit it by using timestamp
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        request.send("username=" + encodeURIComponent(username) +
                     "&password=" + encodeURIComponent(password) +
                     "&apikey=" + encodeURIComponent(apikey));
    }

    function downloadItem(item) {
        if (!item.isDownloaded) {
            item.model.set(item.index, {"isDownloading": true});
            controller.downloadItem(item);
        }
    }
}
