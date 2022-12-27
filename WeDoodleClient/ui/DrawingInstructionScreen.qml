import QtQuick 2.15

Item {
    Rectangle{
        id: background
        anchors.fill: parent
        color: "#91D8E4"
    }

    Component.onCompleted:{
        drawingCanvas.loadImage(gameManager.doodleFilePath())
    }

    Text {
        id: titleText
        text: "Draw:" + gameManager.drawingInstruction + " !"
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
        id:canvasBackground
        anchors.fill: drawingCanvas
        color: "#FFD4B2"
        radius: 15
    }
    Canvas{
        //Not visible we need to put this on a ractangle background
        //For HTML Canvas the topLeft part is origin and horizontal X and vertical Y axis
        id:drawingCanvas
        property int startX : -1
        property int startY: -1
        property int finishX: -1
        property int finishY: -1

        function clearCanvas(){
            var ctx = drawingCanvas.getContext("2d")
            ctx.clearRect(0,0,drawingCanvas.width,drawingCanvas.height)
            requestPaint()
        }

        onImageLoaded: {
            var ctx = drawingCanvas.getContext("2d")
            ctx.drawImage(gameManager.doodleFilePath(),0,0,drawingCanvas.width,drawingCanvas.height)
            drawingCanvas.requestPaint()
        }

        //JS code
        onPaint: {
            var ctx = drawingCanvas.getContext("2d") //getting 2D context
            ctx.beginPath();
            ctx.strokeStyle = "red"
            ctx.lineWidth = 5;
            ctx.lineJoin = "round"
            ctx.moveTo(startX,startY)
            ctx.lineTo(finishX,finishY)
            ctx.closePath()
            ctx.stroke()
            startX = finishX //updating so as to start next drawing from last position
            startY = finishY
        }

        anchors{
            top: titleText.bottom
            bottom: clearButton.top
            topMargin: 20
            bottomMargin: 20
            left: parent.left
            right: parent.right
            leftMargin: 80
            rightMargin: 80
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
                drawingCanvas.startX = mouseX
                drawingCanvas.startY = mouseY
            }
            onPositionChanged: {
                drawingCanvas.finishX = mouseX
                drawingCanvas.finishY = mouseY
                drawingCanvas.requestPaint()
            }
        }
    }


    GameButton{
        id:clearButton
        width: 174
        height: 80
        buttonColor: "#EF5B5B"
        buttonPressedColor: "#b34444"
        buttonText: "Clear"
        buttonTextPixelSize: 36
        anchors{
            left: parent.left
            bottom: parent.bottom
            leftMargin: 80
            bottomMargin: 40

        }
        onButtonClicked: drawingCanvas.clearCanvas()
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
        onButtonClicked: {
            drawingCanvas.save("temp.png")
            mainLoader.source = "qrc:/ui/WaitingForPlayersScreen.qml"
        }

    }
}
