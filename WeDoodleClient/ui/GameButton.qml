import QtQuick 2.15

Rectangle{
    id: gameButton
    property string buttonColor: "#82AAE3"//To change the button colour as needed
    property string buttonText: ""
    property string  buttonPressedColor: "#4482db"
    property int buttonTextPixelSize: 72

    signal buttonClicked()

    radius: 20
    color: buttonColor
    Text{
        anchors.centerIn: parent
        color: "black"
        text: buttonText
        font.pixelSize: buttonTextPixelSize
    }

    MouseArea {
        anchors.fill: parent
        onPressed: gameButton.color = buttonPressedColor
        onReleased: gameButton.color = buttonColor
        onClicked: gameButton.buttonClicked()
    }
}
