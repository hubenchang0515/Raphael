import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

AbstractPage {
    id: mainPage

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
            mainPage.window.x += movement.mouseX - movement.pressX
            mainPage.window.y += movement.mouseY - movement.pressY
        }

        onPressed: press()
        onPositionChanged: move()
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        rows: 2
        columns: 3
        opacity: 0.6

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: log
            text: qsTr("检测文件")
            window: mainPage.window
            onClicked: {
                selectFile.open()
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: clear
            text: qsTr("扫描目录")
            window: mainPage.window

            onClicked: {
                selectDialog.open()
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: clearAll
            text: qsTr("全局扫描")
            window: mainPage.window
            onClicked: {
                toScanPage()
                ClamAV.start()
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: update
            text: qsTr("更新病毒库")
            window: mainPage.window

            onClicked: {
                toUpdatePage()
                Updater.update()
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: source
            text: qsTr("获取源码")
            window: mainPage.window
            onClicked: {
                Qt.openUrlExternally("https://github.com/hubenchang0515/Raphael")
            }
        }

        RaphaelButton {
            Layout.fillWidth: true
            Layout.fillHeight: true

            id: quit
            text: qsTr("退出")
            window: mainPage.window
            onClicked: {
                Qt.quit()
            }
        }
    }

    // 选择扫描目录的窗口
    FileDialog {
        id: selectFile
        title: qsTr("检测文件")
        folder: shortcuts.desktop

        onAccepted: {
            close()
            toScanPage()
            ClamAV.start(fileUrl)
        }
    }

    // 选择扫描目录的窗口
    FileDialog {
        id: selectDialog
        title: qsTr("扫描目录")
        folder: shortcuts.desktop
        selectFolder: true

        onAccepted: {
            close()
            toScanPage()
            ClamAV.start(fileUrl)
        }
    }
}
