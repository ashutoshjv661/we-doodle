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
        text: "Lobby Code: "+ gameManager.roomLobbyCode
        //roomLobbyCode is a Qt property therefore there is no parenthesis. If it was a getter there would have been parenthesis
    }
    Rectangle{
        id: roomLobbyListBackground
        radius : 20
        color : "#BFEAF5"
        height: 418
        width: 358
        anchors{
            top: titleText.bottom
            left: parent.left
            topMargin: 20
            leftMargin: 40
        }
    }
    //List of clients
    ListView{
        id:roomLobbyList
        model: gameManager.clientsInLobby //clientsInLobby is a QT_PROPERTY if any change in list then this will be updated
        delegate: Text{
            id: userID
            anchors.horizontalCenter: parent.horizontalCenter
            text: modelData  //Will iterate through the model list
            font.pixelSize: 36
            font.bold: true
            Image {
                id: checkImage
                visible: gameManager.isClientReady(modelData);
                Connections{
                    target: gameManager
                    onReadyListChanged :checkImage.visible = gameManager.isClientReady(modelData)
                }

                anchors{
                    left: userID.right
                    leftMargin: 15
                    verticalCenter: userID.verticalCenter
                 }
                source: "qrc:/ui/assets/check.png"
                width: 40
                height: 40
                fillMode: Image.PreserveAspectFit
            }
        }
        anchors.fill: roomLobbyListBackground
    }

    Rectangle{
        id: messageWindowBackground
        radius : 20
        color : "#BFEAF5"
        anchors{
            top: roomLobbyListBackground.top
            bottom: roomLobbyListBackground.bottom
            left: roomLobbyListBackground.right
            right: parent.right
            rightMargin: 20
            leftMargin: 40
        }
    }


    TextEdit{
        id:messageWindow
        anchors.fill: messageWindowBackground
        font.pixelSize: 24
        readOnly: true
    }
    Connections{
        target: gameManager
        function onNewLobbyMessage(message){
            messageWindow.append(message)
        }
    }

    GameButton{
        id:readyButton
        buttonText: "Ready"
        buttonTextPixelSize: 36
        width: 318
        height: 80
        anchors{
            top:roomLobbyListBackground.bottom
            topMargin: 20
            horizontalCenter: roomLobbyListBackground.horizontalCenter
        }
        onButtonClicked: {
            gameManager.readyToPlay()
        }
    }
    GameButton{
        id:sendTextButton
        buttonText: "Send"
        buttonTextPixelSize: 36
        width: 174
        anchors{
            top:readyButton.top
            bottom:readyButton.bottom
            right: messageWindowBackground.right
        }
        onButtonClicked: {
            gameManager.sendMessageToLobby(sendTextInput.text)
            sendTextInput.text = ""
        }
    }

    Rectangle{
        id: sendTextFieldBackground
        radius : 20
        color : "#BFEAF5"
        anchors{
            top: readyButton.top
            bottom: readyButton.bottom
            left: messageWindowBackground.left
            right: sendTextButton.left
            rightMargin: 20
        }
    }
    TextInput{
        id:sendTextInput
        anchors.fill: sendTextFieldBackground
        leftPadding: 20
        font.pixelSize: 30
        color: "black"
        clip: true
        verticalAlignment: Text.AlignVCenter
        onAccepted: {//Typing enter key
            gameManager.sendMessageToLobby(sendTextInput.text)
            sendTextInput.text = ""
        }

    }
}
