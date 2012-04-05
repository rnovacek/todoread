import QtQuick 1.1

import com.nokia.meego 1.0

Item {
    id: loginDialog
    signal logIn(string username, string password)
    Column {
        spacing: 3
        anchors.fill: parent
        Grid {
            spacing: 3
            columns: 2
            Label {
                id: usernameLlb
                text: "Username:"
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: username
                width: 200
                height: 50
            }
            Label {
                id: passwordLbl
                text: "Password:"
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: password
                width: 200
                height: 50
            }
        }
        Button {
            id: ok
            text: "Login"

            onClicked: {
                loginDialog.logIn(username.text, password.text)
            }
        }
    }
}
