import QtQuick 2.8
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtQuick.Controls 2.12

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
            }
            Menu {
                title: "Edit"
            }
            Menu {
                title: "Window"
            }
            Menu {
                title: "Build"
            }
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: neneRuntime ? "ViewportRuntime.qml" : "ViewportDesign.qml"
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.25}
}
##^##*/
