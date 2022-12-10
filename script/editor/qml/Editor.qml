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
        NeneRenderItem
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
