import QtQuick 2.15

Item {
    id: gameSelectScreen
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
        id: createGameButtom
        anchors{
            top:titleText.bottom
            topMargin: 160
            horizontalCenter: parent.horizontalCenter
        }

        buttonText: "Create Game"
        buttonTextPixelSize: 48
        width: 336
        height: 105
        onButtonClicked: gameManager.createGameRequest()
        //signal is buttonClicked()
    }
    GameButton{
        id: joinGameButtom
        anchors{
            top:createGameButtom.bottom
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }

        buttonText: "Join Game"
        buttonTextPixelSize: 48
        width: 336
        height: 105
        onButtonClicked: mainLoader.source = "qrc:/ui/JoinLobbyScreen.qml"
        //signal is buttonClicked()
    }
    GameButton{
        id: backButton
        anchors{
            top:joinGameButtom.bottom
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }

        buttonText: "Back"
        buttonTextPixelSize: 48
        width: 336
        height: 105
        onButtonClicked: mainLoader.source = "qrc:/ui/MainMenuScreen.qml"
        //signal is buttonClicked()
    }

}
