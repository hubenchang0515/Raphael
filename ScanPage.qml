import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

AbstractPage {
    id: scanPage
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
            scanPage.window.x += movement.mouseX - movement.pressX
            scanPage.window.y += movement.mouseY - movement.pressY
        }

        onPressed: press()
        onPositionChanged: move()
    }

    Rectangle {
        color: "#333333"
        opacity: 0.8
        anchors.margins: 5
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 5

            Text {
                id: title
                property int virus : 0

                text: virus == 0 ? qsTr("暂未发现异常") : qsTr("异常数量 : ") + virus
                color: virus == 0 ? "green" : "red"

                font.bold: true
                font.pixelSize: 72
                Layout.fillWidth: true
            }

            Text {
                id: state
                Layout.fillWidth: true
                text: qsTr("正在加载病毒数据库...")
                color: "white"
                elide: Text.ElideMiddle // 长度溢出时，中间显示省略号
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#222222"

                ListView{
                    anchors.fill: parent
                    clip: true

                    model: ListModel{
                        ListElement {
                            file: "xxxxxx.xxx"
                            safe: true
                        }

                        ListElement {
                            file: "xxxxxxxxxxxxxxxxxxxx.xxx"
                            safe: true
                        }

                        ListElement {
                            file: "xxxxxx.xxx"
                            safe: false
                        }
                    }

                    delegate:Row {
                        spacing: 10
                        Text {
                            color: "white"
                            text: file
                        }

                        Text {
                            color: safe ? "green" : "red"
                            text: safe ? qsTr("安全") : qsTr("危险")
                        }
                    }
                }
            }


            RaphaelButton {
                id: cancel
                window: scanPage.window

                text: qsTr("取消并返回")

                onClicked: {
                    state.text = qsTr("正在停止扫描...")
                    ClamAV.cancel()
                }
            }
        }
    }

    Connections {
        target: ClamAV
        onDetecting: function(file){
            state.text = qsTr("正在扫描  ") + file
        }

        onAbandon: {
            toMainPage()
        }
    }

}
