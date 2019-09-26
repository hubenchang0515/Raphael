import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: button
    text: qsTr("Button")
    transformOrigin: Item.Center
    highlighted: true
    font.bold: true
    font.family: "Tahoma"

    MouseArea {
        id: movement
        anchors.fill: parent
        property real pressX
        property real pressY
        property bool click: false
    }

    Connections {
        target: movement

        onPressed: {
            movement.pressX = movement.mouseX
            movement.pressY = movement.mouseY
            button.down = true
            movement.click = true
        }

        onPositionChanged: {
            mainWindow.x += movement.mouseX - movement.pressX
            mainWindow.y += movement.mouseY - movement.pressY
            movement.click = false
        }

        onReleased: {
            if(movement.click === true)
            {
                button.clicked()
            }

            button.down = false
            movement.click = false
        }
    }
}

