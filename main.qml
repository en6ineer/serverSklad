import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Server Sklad")

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: urlText
            text: "URL: " + Qt.platform.os + ":8080"
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextArea {
            id: requestOutput
            width: parent.width * 0.8
            height: parent.height * 0.6
            readOnly: true
            placeholderText: "Received POST requests will appear here"
        }
    }
}
