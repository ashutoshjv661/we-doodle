import QtQuick 2.15

Item {
    id: votingScreen
    property string votedImage: ""
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }
    Text {
        id: titleText
        text: qsTr("Vote for the best image!")
        font.pixelSize: 72
        font.bold: true
        anchors{
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }
        color: "black"
    }
    GridView{
        id: imagesGrid
        anchors{
            top: titleText.bottom
            topMargin: 20
            bottom: voteButton.top
            bottomMargin: 20
            left: parent.left
            right: parent.right
            leftMargin: 40
            rightMargin: 40
        }
        clip: true
        model: gameManager.drawingList
        cellWidth:585
        cellHeight:410
        delegate: Rectangle{
            color: "#FFD4B2"
            width: 575
            height: 400
            border.color: "#20A39E"
            border.width: {
                if(votingScreen.votedImage === modelData){
                    return 5;
                }else{
                    return 0;
                }
            }

            Image {
                source: modelData
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
            }
            MouseArea{
                anchors.fill: parent
                onClicked:votingScreen.votedImage = modelData
            }
        }
    }
    GameButton{
        id:voteButton
        width: 174
        height: 80
        buttonColor: "#20A39E"
        buttonPressedColor: "#126663"
        buttonText: "Vote!"
        buttonTextPixelSize: 36
        anchors{
            right: parent.right
            bottom: parent.bottom
            rightMargin: 80
            bottomMargin: 40

        }
        onButtonClicked:{
            gameManager.casteVote(votingScreen.votedImage)
            mainLoader.source = "qrc:/ui/WaitingForPlayersScreen.qml"
        }



    }
}

