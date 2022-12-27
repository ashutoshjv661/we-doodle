import QtQuick 2.15

Item {
    id: mainMenuScreen
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }
    Text {
        id: titleText
        text: qsTr("WeDoodle")
        font.pixelSize: 72
        font.bold: true
        anchors{
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }
        color: "black"
    }
    GameButton{
        id: playButton
        anchors.centerIn: parent
        buttonText: "Play"
        width: 336
        height: 105
        onButtonClicked: mainLoader.source = "qrc:/ui/GameSelectScreen.qml"
        //signal is buttonClicked()
    }
    GameButton{
        id: quitButton
        anchors{
            top: playButton.bottom
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }

        buttonText: "Quit"
        width: 336
        height: 105
        onButtonClicked: Qt.quit() //To exit the game
    }
}
