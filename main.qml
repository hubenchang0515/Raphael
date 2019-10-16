import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Raphael")
    flags: Qt.FramelessWindowHint | Qt.WindowSystemMenuHint
           | Qt.WindowMinimizeButtonHint| Qt.Window;
    color: "Black"

    Image {
        z: -1
        anchors.centerIn: parent
        source: "ClamAV.png"
    }

    MouseArea{
        id: movement
        anchors.fill: parent
        property real pressX
        property real pressY

        function press()
        {
            movement.pressX = movement.mouseX
            movement.pressY = movement.mouseY
        }

        function move()
        {
            mainWindow.x += movement.mouseX - movement.pressX
            mainWindow.y += movement.mouseY - movement.pressY
        }

        onPressed: press()
        onPositionChanged: move()
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        rows: 2
        columns: 3
        opacity: 0.8

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: clear
            text: qsTr("查杀")
            window: mainWindow
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: clearAll
            text: qsTr("全盘查杀")
            window: mainWindow
            onClicked: {
                ClamAV.detect("D:/");
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: update
            text: qsTr("更新病毒库")
            window: mainWindow
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: log
            text: qsTr("查杀日志")
            window: mainWindow
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: source
            text: qsTr("获取源码")
            window: mainWindow
            onClicked: {
                Qt.openUrlExternally("https://github.com/hubenchang0515/Raphael")
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: quit
            text: qsTr("退出")
            window: mainWindow
            onClicked: {
                Qt.quit()
            }
        }
    }
}
