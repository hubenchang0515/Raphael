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
                    id: listView
                    anchors.fill: parent
                    clip: true

                    model: ListModel{
                        id: messageModel
                    }

                    delegate: Row {
                        width: parent.width
                        spacing: 10
                        Text {
                            width: parent.width
                            color: "white"
                            text: message
                            elide: Text.ElideMiddle // 长度溢出时，中间显示省略号
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
        onOpened: {
            state.text = qsTr("病毒数据库加载完毕")
        }

        onFinished: {
            state.text = qsTr("扫描完毕")
        }

        onDetecting: function(file){
            state.text = qsTr("正在扫描  ") + file
        }

        onDetected: function(file, isSafe, virus){
            var text
            if(isSafe)
            {
                text = "<font color='green'>" + qsTr("安全 ") + "</font> : " +  file
            }
            else
            {
                text = "<font color='red'>" + qsTr(" 病毒(") + virus + ")</font> : " + file
            }
            messageModel.append({"message": text})
        }

        onMessage: function(text){
            messageModel.append({"message": text})
        }

        onSent: {
            listView.positionViewAtIndex(listView.count - 1, ListView.Beginning)
        }

        onAbandon: {
            toMainPage()
        }
    }

}
