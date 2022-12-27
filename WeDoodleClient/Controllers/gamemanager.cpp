#include "gamemanager.h"
#include <QFile>
#include <QDir>
#include <QDebug>

GameManager::GameManager(QObject *parent)
    : QObject{parent}
    ,m_isDrawing(false)
    ,m_voteFinised(false)
    ,m_clientID(QString())
    ,m_clientsInLobby(QStringList())
    , m_drawingInstruction(QString())
    , m_drawingList(QStringList())
    , m_winner(QString())
    , m_roomLobbyCode(QString())
    , m_readyClientsList(QStringList())
{
    m_messageHandler = new MessageProcessHandler(this);
    connect(m_messageHandler,&MessageProcessHandler::uniqueIDRegistration,this,&GameManager::registerUniqueID);
    connect(m_messageHandler,&MessageProcessHandler::newLobby,this,&GameManager::lobbyJoined);
    connect(m_messageHandler,&MessageProcessHandler::lobbyListUpdated,this,&GameManager::setClientsInLobby);
    connect(m_messageHandler,&MessageProcessHandler::newLobbyMessage,this,&GameManager::newLobbyMessage); //whenever messageprocess handler has a new lobby message , message process handler processes as its own signal
    connect(m_messageHandler,&MessageProcessHandler::readyListChanged,this,&GameManager::newClientReadyList);
    connect(m_messageHandler,&MessageProcessHandler::gameStarting,this,&GameManager::gameStarting); //to trigger gameStarting in QML
    connect(m_messageHandler,&MessageProcessHandler::doodleAndInstructionReady,this,&GameManager::doodleAndInstructionReady);
    connect(m_messageHandler,&MessageProcessHandler::gameDrawingsReady,this,&GameManager::gameDrawingsReady);
    connect(m_messageHandler,&MessageProcessHandler::winnerChosen,this,&GameManager::winnerChosen);

}

GameManager::~GameManager(){
    m_messageHandler->deleteLater();
}
QString GameManager::roomLobbyCode()
{
    return m_roomLobbyCode;
}

QStringList GameManager::clientsInLobby()
{
    return m_clientsInLobby;
}

QString GameManager::drawingInstruction()
{
    return m_drawingInstruction;
}

QStringList GameManager::drawingList()
{
    return m_drawingList;
}

QString GameManager::winner()
{
    return m_winner;
}

void GameManager::createGameRequest()
{
    //type:createGame;payLoad:0;sender:"+m_clientID
    emit newMessageReadyToSend("type:createGame;payLoad:0;sender:"+m_clientID);

}

void GameManager::joinLobbyRequest(QString lobbyID)
{
    //type:joinGame;payLoad:4000;sender:5555
    emit newMessageReadyToSend("type:joinGame;payLoad:"+lobbyID+";sender:"+m_clientID);
}

void GameManager::sendMessageToLobby(QString message)
{
     //type:message;payLoad:HelloWorld;lobbyID:8888;sender:5555;
    emit newMessageReadyToSend("type:message;payLoad:"+message+";lobbyID:"+m_roomLobbyCode+";sender:"+m_clientID);
}

bool GameManager::isClientReady(QString clientID)
{
    return m_readyClientsList.contains(clientID);
}

void GameManager::readyToPlay()
{
    //type:readyToPlay;payLaod:1;sender:5555
    emit newMessageReadyToSend("type:readyToPlay;payLoad:1;sender:"+m_clientID);
}

void GameManager::doodleDone()
{

    if(m_voteFinised) return;
    //open canvas image ->convert to qbytearray -> create data packet -> and send to server
    QFile imageFile("temp.png");
    if( !imageFile.open(QIODevice::ReadOnly)){
        return; //Implement further if we are unable to open a file we can redirect back to scribble screen
    }
    QByteArray fileData = imageFile.readAll();
    imageFile.close();

    QString dataPacket = QString();
    if(m_isDrawing){
        //drawDataType
        //type:drawData;payLoad:fileData;sender:clientID
        dataPacket = "type:drawData;payLoad:"+fileData.toHex() +";sender:"+m_clientID;
    }else{
        //doodleDataType
        //type:doodleData;payLoad:fileData;sender:clientID

        dataPacket = "type:doodleData;payLoad:"+fileData.toHex() +";sender:"+m_clientID;
    }
    emit newMessageReadyToSend(dataPacket);
}

QString GameManager::doodleFilePath()
{
    QString localPath = QDir::currentPath();
    //TODO

    QString ret = "file:///"+localPath+QDir::separator()+m_clientID+".png";
    qDebug() <<"Opening path : "<< ret;
    return ret;
}

void GameManager::casteVote(QString imageURL)
{
    m_voteFinised = true;
    //file:///path/temp/clientID.png
    QFileInfo fileInfo(imageURL);
    qDebug() << fileInfo.baseName();
    //type:casteVote;payLoad:clientID;sender:1111
    emit newMessageReadyToSend("type:casteVote;payLoad:"+ fileInfo.baseName()+";sender:"+m_clientID);
}

QString GameManager::winnerImage()
{
    QDir workingDir = QDir::currentPath();
    workingDir.mkdir("temp");
    QString filePath = QDir::currentPath() + QDir::separator() + "temp" + QDir::separator() + m_winner +".png" ;
    filePath.prepend("file:///");
    return filePath;

}

void GameManager::setRoomLobbyCode(QString lobbyCode){
    if(m_roomLobbyCode!=lobbyCode){
        m_roomLobbyCode = lobbyCode;
        emit roomLobbyCodeChanged();
    }
}

void GameManager::setClientsInLobby(QStringList clientList)
{
    if(m_clientsInLobby != clientList){
        m_clientsInLobby = clientList;
        emit clientsInLobbyChanged();
    }
}

void GameManager::setDrawingInstruction(QString instruction)
{
    if(m_drawingInstruction!=instruction){
        m_drawingInstruction = instruction;
        emit drawingInstructionChanged();
    }
}

void GameManager::setDrawingList(QStringList newList)
{
    if(m_drawingList != newList){
        m_drawingList = newList;
        emit drawingListChanged();
    }
}

void GameManager::setWinner(QString winner)
{
    if(m_winner != winner){
        m_winner = winner;
        emit winnerChanged();
    }
}

void GameManager::processSocketMessage(QString message)
{
    m_messageHandler->processMessage(message);
}

void GameManager::registerUniqueID(QString uniqueID)
{
    qDebug() << "Client App : Registering unique id :" + uniqueID;
    m_clientID = uniqueID;
}

void GameManager::lobbyJoined(QString lobbyID,QStringList clients)
{
    setRoomLobbyCode(lobbyID);
    setClientsInLobby(clients);
    emit inGameLobby();
}

void GameManager::newClientReadyList(QStringList readyClients)
{
    if(m_readyClientsList != readyClients){
        m_readyClientsList = readyClients;
        emit readyListChanged();
    }
}

void GameManager::doodleAndInstructionReady(QString data, QString instruction)
{
    setDrawingInstruction(instruction);
    m_isDrawing = true;
    QFile tempImage(m_clientID + ".png");
    if(!tempImage.open(QIODevice::WriteOnly))
        return;
    QByteArray fileContents = QByteArray::fromHex(data.toLocal8Bit()); //toUtf8
    tempImage.write(fileContents);
    tempImage.flush();
    tempImage.close();
    emit drawInstructionTime();
}

void GameManager::gameDrawingsReady(QStringList images, QStringList clients)
{
    QDir workingDir = QDir::currentPath();
    workingDir.mkdir("temp");
    QString filePath = QDir::currentPath() + QDir::separator() + "temp" + QDir::separator() ;
    QStringList newFiles;

    for(int i=0;i<clients.size();i++){
        QString fileName = filePath + clients.at(i) + ".png";
        QFile tempImage(fileName);
        tempImage.open(QIODevice::WriteOnly);
        QByteArray fileData = QByteArray::fromHex(images.at(i).toLocal8Bit());
        tempImage.write(fileData);
        tempImage.flush();
        tempImage.close();
        fileName.prepend("file:///");
        newFiles.append(fileName);
    }
    setDrawingList(newFiles);
    emit votingTime();
}

void GameManager::winnerChosen(QString winner){
    setWinner(winner);
    emit gameOver();
}
