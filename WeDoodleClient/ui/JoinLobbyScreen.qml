import QtQuick 2.15

Item {
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }
    Text {
        id: titleText
        font.pixelSize: 72
        font.bold: true
        anchors{
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }
        color: "black"
        text: "Enter Game Code"
        //roomLobbyCode is a Qt property therefore there is no parenthesis. If it was a getter there would have been parenthesis
    }
    Rectangle{
        id: joinLobbyTextBackground
        radius : 20
        color : "#BFEAF5"
        width: 618
        height: 79
        anchors{
            horizontalCenter: parent.horizontalCenter
            top: titleText.bottom
            topMargin: 100
        }
    }
    TextInput {
        id:joinLobbyTextInput
        anchors.fill: joinLobbyTextBackground
        color: "black"
        font.pixelSize: 48
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        maximumLength: 4
        inputMask: "9999"
    }
    GameButton{
        id:joinButton
        width:336
        height: 105
        buttonText: "Join Game"
        buttonTextPixelSize: 48
        anchors{
            horizontalCenter: parent.horizontalCenter
            top : joinLobbyTextBackground.bottom
            topMargin: 100
        }
        onButtonClicked: {
            if(joinLobbyTextInput.text !== "")//  !== ""
                gameManager.joinLobbyRequest(joinLobbyTextInput.text)
        }

    }
    GameButton{
        id:backButton
        width:336
        height: 105
        buttonText: "Back"
        buttonTextPixelSize: 48
        anchors{
            horizontalCenter: parent.horizontalCenter
            top : joinButton.bottom
            topMargin: 20
        }
        onButtonClicked: mainLoader.source = "qrc:/ui/GameSelectScreen.qml"
    }
}
