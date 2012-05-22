import QtQuick 1.1

import com.nokia.meego 1.0

Page {
    anchors.margins: 16

    Item {
        width: parent.width;
        Label {
            id: lbl
            text: "Show favicons";
            anchors.left: parent.left
        }
        Switch {
            id: showFavicons;
            anchors.right: parent.right
        }
    }


    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

}
