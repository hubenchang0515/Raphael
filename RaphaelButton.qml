import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5

Button {
    id: button
    text: qsTr("Button")
    transformOrigin: Item.Center
    highlighted: true
    font.bold: true
    font.family: "Tahoma"
    property Window window

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
            if(window == null)
            {
                return
            }

            if(movement.mouseX - movement.pressX < 5 &&
                    movement.mouseY - movement.pressY)
            {
                return
            }

            window.x += movement.mouseX - movement.pressX
            window.y += movement.mouseY - movement.pressY
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

