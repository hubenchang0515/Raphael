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

    Loader {
        id: pageLoader
        anchors.fill: parent
        sourceComponent: MainPage {
            window: mainWindow
        }
    }

    Connections {
        target: pageLoader.item

        onToMainPage: {
            pageLoader.setSource("MainPage.qml", {"window": mainWindow})
        }

        onToScanPage: {
            pageLoader.setSource("ScanPage.qml", {"window": mainWindow})
        }
    }
}
