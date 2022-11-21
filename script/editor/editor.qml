import QtQuick 2.8
import QtNene 1.0


Item {
   Rectangle {
        id: clipper
        width: parent.width / 2
        height: parent.height / 2
        anchors.centerIn: parent
        border.color: "yellow"
        border.width: 2
        color: "transparent"
        NumberAnimation on rotation {
            id: nonRectClipAnim
            from: 0; to: 360; duration: 5000; loops: Animation.Infinite
            running: false
        }

        //! [3]
        CustomRenderItem {
            id: renderer
            width: parent.width / 2 - 20
            height: parent.height / 2 - 20
            x: -clipper.x + 10
            y: -clipper.y + 10

            transform: [
                Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: renderer.width / 2; origin.y: renderer.height / 2; },
                Translate { id: txOut; x: -renderer.width / 2; y: -renderer.height / 2 },
                Scale { id: scale; },
                Translate { id: txIn; x: renderer.width / 2; y: renderer.height / 2 }
            ]
        }
        //! [3]
   }
}
