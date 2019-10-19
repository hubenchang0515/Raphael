import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

AbstractPage {
    id: resultPage
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
            resultPage.window.x += movement.mouseX - movement.pressX
            resultPage.window.y += movement.mouseY - movement.pressY
        }

        onPressed: press()
        onPositionChanged: move()
    }

    Component {
        id: selectBox
        CheckBox {
            property int row: -1
            property var select
            opacity: 1

            onClicked: {
                select(checkState == Qt.Checked, row)
            }
        }
    }

    Component {
        id: fileDisplay
        Item {
            property string filename
            implicitHeight: 50

            Text {
                text: filename
                color: "white"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle // 长度溢出时，中间显示省略号
            }
        }
    }

    Component {
        id: virusDisplay
        Item {
            property string virusname
            implicitHeight: 50
            Text {
                text: virusname
                color: "red"
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle // 长度溢出时，中间显示省略号
            }
        }
    }

    Rectangle {
        color: "#333333"
        opacity: 0.6
        anchors.margins: 5
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 5

            Text {
                id: title
                property int virus : 0

                text: Virus.rowCount() === 0 ? qsTr("暂未发现异常") : qsTr("异常数量 : ") + Virus.rowCount()
                color: Virus.rowCount() === 0 ? "green" : "red"

                font.bold: true
                font.pixelSize: 72
                Layout.fillWidth: true
            }

            Text {
                id: state
                Layout.fillWidth: true
                text: qsTr("扫描完毕")
                color: "white"
                elide: Text.ElideMiddle // 长度溢出时，中间显示省略号
            }

            Connections {
                target: Virus

                onCleanFinished: {
                    toResultPage()
                }

                onRemove: function(file){
                    state.text = qsTr("正在删除") + file
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#222222"

                TableView {
                    anchors.fill: parent
                    columnSpacing: 5
                    rowSpacing: 5
                    clip: true

                    model: Virus

                    delegate: Loader{
                        width: 300
                        sourceComponent: {
                            switch(column)
                            {
                                case 0: return selectBox;
                                case 1: return virusDisplay;
                                case 2: return fileDisplay;
                            }
                        }

                        onLoaded: {
                            switch(column)
                            {
                                case 0:
                                    item.checkState = selected ? Qt.Checked : Qt.Unchecked
                                    item.row = row
                                    item.select = Virus.select
                                    break
                                case 1:
                                    item.virusname = display
                                    item.implicitWidth = 140
                                    break
                                case 2:
                                    item.filename = display
                                    item.implicitWidth = parent.width - 200
                                    break
                            }

                        }
                    }
                }
            }

            Row {
                spacing: 10
                RaphaelButton {
                    id: clear
                    window: resultPage.window
                    text: qsTr("删除")
                    onClicked: {
                        Virus.clean()
                    }
                }

                RaphaelButton {
                    id: cancel
                    window: resultPage.window

                    text: qsTr("返回")

                    onClicked: {
                        Virus.clear()
                        toMainPage()
                    }
                }
            }
        }
    }
}
