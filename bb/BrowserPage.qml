
import bb.cascades 1.0

Page {
    id: browserPage
    property variant item

    function setItem(it) {
        item = it
        browser.url = item.url;
    }

    ScrollView {
        id: scrollView
        scrollViewProperties {
            scrollMode: ScrollMode.Both
            pinchToZoomEnabled: true
        }
        layoutProperties: StackLayoutProperties { spaceQuota: 1.0 }

        Container {
            background: Color.LightGray

            WebView {
                id: browser
                url: "about:blank"

                onMinContentScaleChanged: {
                    scrollView.scrollViewProperties.minContentScale = minContentScale;
                }

                onMaxContentScaleChanged: {
                    scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                }
            }
        }
    }

    actions: [
         ActionItem {
            title: item.status == 0 ? "Mark as read" : "Unmark as read"
            imageSource: item.status == 0 ? "unread_dark.png" : "read_dark.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (item.status == 0) {
                    item.markAsRead();
                } else {
                    item.unmarkAsRead();
                }
            }
        },
        ActionItem {
            title: "Open in browser"
            imageSource: "browser_dark.png"
            onTriggered: {
                controller.openExternal(browser.url)
            }
        }
    ]
}
