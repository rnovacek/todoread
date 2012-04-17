import QtQuick 1.1

import com.nokia.meego 1.0

Page {
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
    }

    Flickable {
        contentHeight: text.height
        anchors.margins: 12
        anchors.fill: parent
        Label {
            id: text
            text: "
    <h1>ToDo:Read</h1>
    <p><i>ToDo:Read is application for reading articles you have postponed.</i></p>
    <p>Author: Radek Nováček <a href=\"mailto:rad.n@centrum.cz\">mail</a></p>
    <p><a href=\"http://rnovacek.github.com/todoread/\">Home page</a></p>
    <p>If you don't know how to use this application, visit <a href=\"https://github.com/rnovacek/todoread/wiki/Getting-Started\">Getting Started</a> page.</p>
    <p>Please report any issues and feature requests to the <a href=\"https://github.com/rnovacek/todoread/issues\">Issue tracker</a>.</p>
    <p>Or you can check out the <a href=\"https://github.com/rnovacek/todoread\">source code</a>.</p>
    "
            width: parent.width
            wrapMode: Text.WordWrap
            onLinkActivated: controller.openExternal(link)
        }
    }
}
