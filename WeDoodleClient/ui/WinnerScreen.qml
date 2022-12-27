import QtQuick 2.15

Item {
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }
    Text {
        id: titleText
        text: gameManager.winner + " was the best!"
        font.pixelSize: 72
        font.bold: true
        anchors{
            top: parent.top
            topMargin: 40
            horizontalCenter: parent.horizontalCenter
        }
        color: "black"
    }
    Rectangle{
        anchors.fill: winnerImage
        color: "#20A39E"
    }

    Image{
        id:winnerImage
        anchors{
            top:titleText.bottom
            bottom:doneButton.top
            left:parent.left
            right:parent.right
            margins: 20
        }
        source: gameManager.winnerImage()
    }

    GameButton{
        id:doneButton
        width: 174
        height: 80
        buttonColor: "#20A39E"
        buttonPressedColor: "#126663"
        buttonText: "Done"
        buttonTextPixelSize: 36
        anchors{
            right: parent.right
            bottom: parent.bottom
            rightMargin: 80
            bottomMargin: 40

        }
        onButtonClicked:Qt.quit()

    }
}
