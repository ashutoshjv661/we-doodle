import QtQuick 2.15
import QtQml 2.3
Item {
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }

    Component.onCompleted: {
        gameManager.doodleDone()
    }

    Text {
        id: titleText
        text: qsTr("Waiting for other players<br> to finish ...")
        font.pixelSize: 72
        font.bold: true
        anchors{
            centerIn: parent
        }
        color: "black"
        horizontalAlignment: Text.AlignHCenter
    }

}
