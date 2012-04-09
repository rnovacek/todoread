.pragma library

var items = new Array;

var itemHash = new Object;

function ReadItem(id, title, url, state) {
    this.id = id;
    this.title = title;
    this.url = url;
    this.origUrl = url;
    this.isRead = state == 1;
    this.isOrigRead = this.isRead;
    this.isDownloaded = false;
    this.isDownloading = false;
    this.model = null;
    this.index = -1;

    itemHash[id] = this;
}

function setValue(item, prop, value) {
    console.log("Set value for item[" + item.id + ": " + item + " " + prop + " value:" + value)
    itemHash[item.id][prop] = value;
    if (item.index >= 0) {
        item.model.setProperty(item.index, prop, value);
    }

}

function load(dump) {
    if (dump.length > 0) {
        items = eval('(' + dump + ')');
        for (var i in items) {
            itemHash[items[i].id] = items[i];
        }
    }
}
