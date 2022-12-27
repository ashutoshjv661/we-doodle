import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("WeDoodle : Scribble for fun")

    Connections {
        target: gameManager
        function onInGameLobby() {
            mainLoader.source = "qrc:/ui/LobbyScreen.qml" //the signal is inGameLobby
        }
        function onGameStarting(){
            mainLoader.source = "qrc:/ui/DoodleScreen.qml"
        }
        function onDrawInstructionTime(){
            mainLoader.source = "qrc:/ui/DrawingInstructionScreen.qml"
        }
        function onVotingTime(){
            mainLoader.source = "qrc:/ui/VoteScreen.qml"
        }
        function onGameOver(){
            mainLoader.source = "qrc:/ui/WinnerScreen.qml"
        }
    }

    Loader{
        id: mainLoader
        anchors.fill: parent
        source: "qrc:/ui/MainMenuScreen.qml"
    }
}
