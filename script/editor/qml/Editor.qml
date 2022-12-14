import QtQuick 2.8
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtQuick.Controls 2.12
import QtNene 1.0

Window {
    property bool neneRuntime: false
    property string windowTitle: "Title"

    visible: true
    width: 1920
    height: 1080
    color: "darkgray"
    title: windowTitle

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        MenuBar {
            Layout.fillWidth: true

            Menu {
                title: "File"
                
                Action { text: "New Level" }
                Action { text: "Open Level" }
                Action { text: "Close Level" }
                Action { text: "Close Asset" }
                
                MenuSeparator { }
                
                Action { text: "Import Asset" }
                Action { text: "Export Asset" }
            }
            Menu {
                title: "Edit"

                Action { text: "Undo" }
                Action { text: "Redo" }
                Action { text: "History" }

                MenuSeparator { }

                Action { text: "Cut" }
                Action { text: "Copy" }
                Action { text: "Paste" }
                Action { text: "Delete" }
            }
            Menu {
                title: "Window"
            }
            Menu {
                title: "Build"
            }
        }

        Rectangle {
            id: clipper
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "yellow"
            border.width: 2
            color: "blue"
            clip: false
            NumberAnimation on rotation {
                id: nonRectClipAnim
                from: 0; to: 360; duration: 10000; loops: Animation.Infinite
                running: false
            }

            NeneRenderItem {
                id: renderer
                width: clipper.width - 20
                height: clipper.height - 20
                opacity: 1.0

                transform: [
                    Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: renderer.width / 2; origin.y: renderer.height / 2; },
                    Translate { id: txOut; x: -renderer.width / 2; y: -renderer.height / 2 },
                    Scale { id: scale; },
                    Translate { id: txIn; x: renderer.width / 2; y: renderer.height / 2 }
                ]
            }

            Text {
                id: label2
                anchors.top: parent.top
                anchors.right: parent.right
                color: "yellow"
                text: "Clip: " + (clipper.clip ? "ON" : "OFF") + " Rotation: " + (nonRectClipAnim.running ? "ON" : "OFF")
            }
        }
    }

}
