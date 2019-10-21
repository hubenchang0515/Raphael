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
        opacity: 0.6
        anchors.margins: 5
        anchors.fill: parent

        GridLayout{
            anchors.fill: parent
            columns: 2
            anchors.margins: 20

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    id: mainTitle
                    text: qsTr("main.cvd")
                    color: "white"
                    font.pixelSize: 32

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    property int numerator: 0   // 分子
                    property int denominator: 0 // 分母

                    id: mainState
                    text: numerator + "/" + denominator
                    color: "white"

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            ProgressBar {
                id: mainProgressBar
                Layout.fillWidth: true
                Layout.fillHeight: true

                property int numerator: 0   // 分子
                property int denominator: 1 // 分母
                from: 0
                to: denominator
                value: numerator
            }

            ColumnLayout {
                Text {
                    id: dailyTitle
                    text: qsTr("daily.cvd")
                    color: "white"
                    font.pixelSize: 32
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    property int numerator: 0   // 分子
                    property int denominator: 0 // 分母

                    id: dailyState
                    text: numerator + "/" + denominator
                    color: "white"

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            ProgressBar {
                id: dailyProgressBar
                Layout.fillWidth: true
                Layout.fillHeight: true

                property int numerator: 0   // 分子
                property int denominator: 1 // 分母

                from: 0
                to: denominator
                value: numerator
            }

            ColumnLayout {
                Text {
                    id: bytecodeTitle
                    text: qsTr("bytecode.cvd")
                    color: "white"
                    font.pixelSize: 32
                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    property int numerator: 0   // 分子
                    property int denominator: 0 // 分母

                    id: bytecodeState
                    text: numerator + "/" + denominator
                    color: "white"

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            ProgressBar {
                id: bytecodeProgressBar
                Layout.fillWidth: true
                Layout.fillHeight: true

                property int numerator: 0   // 分子
                property int denominator: 1 // 分母

                from: 0
                to: denominator
                value: numerator
            }

            RaphaelButton {
                id: back
                window: scanPage.window
                Layout.alignment: Qt.AlignHCenter

                text: qsTr("取消并返回")

                onClicked: {
                    Updater.stop()
                    toMainPage()
                }
            }
        }
    }

    Connections {
        target: Updater

        onMainProgress: function(value){
            mainState.numerator = value
            mainProgressBar.numerator = value
        }

        onDailyProgress: function(value){
            dailyState.numerator = value
            dailyProgressBar.numerator = value
        }

        onBytecodeProgress: function(value){
            bytecodeState.numerator = value
            bytecodeProgressBar.numerator = value
        }

        onMainLength: function(value){
            mainState.denominator = value
            mainProgressBar.denominator = value
        }

        onDailyLength: function(value){
            dailyState.denominator = value
            dailyProgressBar.denominator = value
        }

        onBytecodeLength: function(value){
            bytecodeState.denominator = value
            bytecodeProgressBar.denominator = value
        }

        onMainFinished: {
            mainTitle.color = "green"
            mainState.color = "green"
        }

        onDailyFinished: {
            dailyTitle.color = "green"
            dailyState.color = "green"
        }

        onBytecodeFinished: {
            bytecodeTitle.color = "green"
            bytecodeState.color = "green"
        }

        onMainDefeated: function(value){
            mainTitle.color = "red"
            mainState.color = "red"
            mainState.text = "QNetworkReply::NetworkError : " + value
        }

        onDailyDefeated: function(value){
            dailyTitle.color = "red"
            dailyState.color = "red"
            dailyState.text = "QNetworkReply::NetworkError : " + value
        }

        onBytecodeDefeated: function(value){
            bytecodeTitle.color = "red"
            bytecodeState.color = "red"
            bytecodeState.text = "QNetworkReply::NetworkError : " + value
        }
    }
}
