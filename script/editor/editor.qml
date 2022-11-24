import QtQuick 2.8
import QtNene 1.0

Rectangle {
    id: page
    width: 320; height: 480
    color: "lightgray"

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }

    Rectangle {
        id: renderRect
        width: 200; height: 200
        color: "yellow"
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        CustomRenderItem {
            id: renderer
            width: 200
            height: 200
            y: 70
        }
    }
}