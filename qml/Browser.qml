/****************************************************************************
** This file is originally from examples from QtDeclarative module of Qt toolkit.
** Pinching support added by KDE Plasma Active guys.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

import QtQuick 1.1
import QtWebKit 1.0

Flickable {
    property alias title: webView.title
    property alias icon: webView.icon
    property alias progress: webView.progress
    property alias url: webView.url
    property alias back: webView.back
    property alias stop: webView.stop
    property alias reload: webView.reload
    property alias forward: webView.forward
    property bool interactiveSuspended: false

    signal newWindowRequested(string url)


    id: flickable
    width: parent.width
    contentWidth: Math.max(parent.width,webView.width)
    contentHeight: Math.max(parent.height,webView.height)
    /*
    interactive: {
        (webView.flickingEnabled &&
         !interactiveSuspended &&
         ((webView.height > height) ||
         (webView.width > width)))
    }
    */
    anchors.top: parent.bottom
    anchors.bottom: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    pressDelay: 200

    onWidthChanged : {
        // Expand (but not above 1:1) if otherwise would be smaller that available width.
        if (width > webView.width*webView.contentsScale && webView.contentsScale < 1.0)
            webView.contentsScale = width / webView.width * webView.contentsScale;
    }


    PinchArea {
        id: pinchArea
        width:webView.width
        height: webView.height
        property real startScale
        property real startY
        property real startX
        onPinchStarted: {
            startScale = webView.contentsScale
            webView.renderingEnabled = false
            flickable.smooth = false
            startY = pinch.center.y
            startX = pinch.center.x
        }
        onPinchUpdated: {
            webView.contentsScale = startScale * pinch.scale

            flickable.contentY += pinch.previousCenter.y - pinch.center.y + startY * (pinch.scale - pinch.previousScale)

            flickable.contentX += pinch.previousCenter.x - pinch.center.x + startX * (pinch.scale - pinch.previousScale)
        }
        onPinchFinished: {
            if (webView.contentsSize.width < webView.width) {
                webView.contentsSize.width = webView.width
            }

            webView.renderingEnabled = true
            flickable.smooth = true
        }
        WebView {
            id: webView
            objectName: "webViewImplementation"
            transformOrigin: Item.TopLeft
            //settings.pluginsEnabled: true

            //FIXME: glorious hack just to obtain a signal of the url of the new requested page
            // Should be replaced with signal from KDeclarativeWebView
            newWindowComponent: Component {
                Item {
                    id: newPageComponent

                    WebView {
                        id: newWindow
                        onUrlChanged: {
                            if (url != "") {
                                flickable.newWindowRequested(url)

                                var newObject = Qt.createQmlObject('import QtQuick 1.0; Item {}', webView);
                                newPageComponent.parent = newObject
                                newObject.destroy()
                            }
                        }
                    }
                }
            }

            newWindowParent: webView

            function fixUrl(url)
            {
                if (url == "") return url
                if (url[0] == "/") return "file://"+url
                if (url.indexOf(":")<0) {
                    if (url.indexOf(".")<0 || url.indexOf(" ")>=0) {
                        // Fall back to a search engine; hard-code Wikipedia
                        return "http://en.wikipedia.org/w/index.php?search="+url
                    } else {
                        return "http://"+url
                    }
                }
                return url
            }

            smooth: false // We don't want smooth scaling, since we only scale during (fast) transitions
            focus: true

            onAlert: {
                console.log(message);
                alertDialog.text = message;
                alertDialog.open();
            }

            function doZoom(zoom,centerX,centerY)
            {
                if (centerX) {
                    var sc = zoom*contentsScale;
                    scaleAnim.to = sc;
                    flickVX.from = flickable.contentX
                    flickVX.to = Math.max(0,Math.min(centerX-flickable.width/2,webView.width*sc-flickable.width))
                    finalX.value = flickVX.to
                    flickVY.from = flickable.contentY
                    flickVY.to = Math.max(0,Math.min(centerY-flickable.height/2,webView.height*sc-flickable.height))
                    finalY.value = flickVY.to
                    quickZoom.start()
                }
            }

            function handleLinkPressed(linkUrl, linkRect)
            {
                print("link pressed: " + linkUrl + " | " + linkRect.x + " " + linkRect.y + " " + linkRect.width + " " + linkRect.height);
    //            flickable.interactiveSuspended = true;
    //             highlightRect.x = linkRect.x;
    //             highlightRect.y = linkRect.y;
    //             highlightRect.width = linkRect.width;
    //             highlightRect.height = linkRect.height;
            }

            function handleLinkPressAndHold(linkUrl, linkRect)
            {
    //            print("... and hold: " + linkUrl + " | " + linkRect.x + " " + linkRect.y + " " + linkRect.width + " " + linkRect.height);
                linkPopupLoader.source = "LinkPopup.qml";
                if (linkPopupLoader.status == Loader.Ready) {
                    flickable.interactiveSuspended = true;
                    highlightRect.x = linkRect.x;
                    highlightRect.y = linkRect.y;
                    highlightRect.width = linkRect.width;
                    highlightRect.height = linkRect.height;

                    var linkPopup = linkPopupLoader.item;
                    linkPopup.url = linkUrl
                    linkPopup.linkRect.x = linkRect.x
                    linkPopup.linkRect.y = linkRect.y
                    linkPopup.linkRect.width = linkRect.width
                    linkPopup.linkRect.height = linkRect.height
                    linkPopup.state  = "expanded";
                    //print(" type: " + typeof(linkRect));
                }
            }

            /*
            Rectangle {
                id: highlightRect
                color: theme.highlightColor
                opacity: 0.2
                visible: (linkPopupLoader.source != "" && linkPopupLoader.item.state == "expanded")
            }
            */

            Loader { id: linkPopupLoader }

            Keys.onLeftPressed: webView.contentsScale -= 0.1
            Keys.onRightPressed: webView.contentsScale += 0.1

            preferredWidth: flickable.width
            preferredHeight: flickable.height
            contentsScale: 1
            onContentsSizeChanged: {
                // zoom out
                contentsScale = Math.min(1,flickable.width / contentsSize.width)
            }
            onUrlChanged: {
                // got to topleft
                flickable.contentX = 0
                flickable.contentY = 0
                //settings.pluginsEnabled = true;
            }
            onTitleChanged: {
                //print("title changed in flickable " + title);
                //webBrowser.titleChanged();
            }
            onDoubleClick: {
                preferredWidth = flickable.width - 50;
                if (!heuristicZoom(clickX,clickY,2.0)) {
                    var zf = flickable.width / contentsSize.width
                    if (zf >= contentsScale)
                        zf = 2.0*contentsScale // zoom in (else zooming out)
                    doZoom(zf,clickX*zf,clickY*zf)
                }
            }

            SequentialAnimation {
                id: quickZoom

                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: false
                }
                PropertyAction {
                    target: flickable
                    property: "smooth"
                    value: false
                }
                ParallelAnimation {
                    NumberAnimation {
                        id: scaleAnim
                        target: webView
                        property: "contentsScale"
                        // the to property is set before calling
                        easing.type: Easing.Linear
                        duration: 200
                    }
                    NumberAnimation {
                        id: flickVX
                        target: flickable
                        property: "contentX"
                        easing.type: Easing.Linear
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                    NumberAnimation {
                        id: flickVY
                        target: flickable
                        property: "contentY"
                        easing.type: Easing.Linear
                        duration: 200
                        from: 0 // set before calling
                        to: 0 // set before calling
                    }
                }
                // Have to set the contentXY, since the above 2
                // size changes may have started a correction if
                // contentsScale < 1.0.
                PropertyAction {
                    id: finalX
                    target: flickable
                    property: "contentX"
                    value: 0 // set before calling
                }
                PropertyAction {
                    id: finalY
                    target: flickable
                    property: "contentY"
                    value: 0 // set before calling
                }
                PropertyAction {
                    target: webView
                    property: "renderingEnabled"
                    value: true
                }
                PropertyAction {
                    target: flickable
                    property: "smooth"
                    value: true
                }
            }
            onZoomTo: doZoom(zoom,centerX,centerY)
            /*
            onLinkPressed: handleLinkPressed(linkUrl, linkRect)
            onLinkPressAndHold: handleLinkPressAndHold(linkUrl, linkRect)
            */
        }
    }

    Component.onCompleted: {
        back.enabled = false
        forward.enabled = false
        reload.enabled = false
        stop.enabled = false
    }
}

