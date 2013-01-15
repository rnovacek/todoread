
import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: "Todo:Read Configuration"
    }
    content: Container {
        Label {
            text: "Show favicons"
        }
        ToggleButton {
            horizontalAlignment: HorizontalAlignment.Right
            checked: controller.configValue("showFavicons", true)
            onCheckedChanged: {
                controller.setConfigValue("showFavicons", checked);
            }
        }
        Label {
            text: "Open articles in external browser"
        }
        ToggleButton {
            horizontalAlignment: HorizontalAlignment.Right
            checked: controller.configValue("useExternalBrowser", false)
            onCheckedChanged: {
                controller.setConfigValue("useExternalBrowser", checked);
            }
        }
        Label {
            text: "Mark article as read when showed"
        }
        ToggleButton {
            horizontalAlignment: HorizontalAlignment.Right
            checked: controller.configValue("markAutomatically", false)
            onCheckedChanged: {
                controller.setConfigValue("markAutomatically", checked);
            }
        }
    }
}
