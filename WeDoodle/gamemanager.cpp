#include "gamemanager.h"
#include <QString>
#include <QMap>
#include <QDebug> //To show debug statements
#include <random> //to generate random numbers

GameManager::GameManager(QObject *parent) : QObject{parent}
{
    m_socketHandler = new WebSocketHandler(this);
    m_messageProcessHandler = new MessageProcessorHandler(this); //parent
    connect(m_socketHandler,&WebSocketHandler::newMessageToProcess,m_messageProcessHandler,&MessageProcessorHandler::processMessage);

    connect(m_messageProcessHandler,&MessageProcessorHandler::createGameRequest,this,&GameManager::createGameLobbyRequest);
    connect(m_messageProcessHandler,&MessageProcessorHandler::joinGameLobbyRequest,this,&GameManager::joinGameLobbyRequest);
    connect(m_messageProcessHandler,&MessageProcessorHandler::messageLobbyRequest,this,&GameManager::messageLobbyRequest);
    connect(m_messageProcessHandler,&MessageProcessorHandler::clientReadyToPlay,this,&GameManager::userReadyToPlay);
    connect(m_messageProcessHandler,&MessageProcessorHandler::newDoodleData,this,&GameManager::newDoodleDataReady);
    connect(m_messageProcessHandler,&MessageProcessorHandler::newDrawData,this,&GameManager::newDoodleDrawReady);
    connect(m_messageProcessHandler,&MessageProcessorHandler::newVote,this,&GameManager::newVote);
}

GameManager::~GameManager(){
    m_socketHandler->deleteLater();
    //Whenever Parent is deleted (Here parent is GameManager) then it will delete the SocketHandler
}

void GameManager::createGameLobbyRequest(QString uniqueID)
{
    std::random_device rd;
    std::default_random_engine generator; //pseudo random generator , after some time numbers are repeated
    generator.seed(rd()); //so we seed it with rd to generate unique value everytime
    std::uniform_int_distribution<int> idGenerator(1000,9999); //generate 4 digit unique value
    QString newLobbyID = QString::number(idGenerator(generator));

    //QMap<QString,QWebsocket*>
    //ensuring that ID is unique
    while(m_gameLobbyMap.keys().contains(newLobbyID)){
        newLobbyID = QString::number(idGenerator(generator));
    }
    GameLobbyHandler * newGameLobby = new GameLobbyHandler(newLobbyID, this);

    //Connection for new player ready so that we can send it to all connected clients
    connect(newGameLobby,&GameLobbyHandler::userReadyListChanged,this,&GameManager::userReadyListChanged);
    //Connection for starting game if all players are ready
    connect(newGameLobby,&GameLobbyHandler::gameReadyToBegin,this,&GameManager::gameReadyToBegin);
    //Connection when all doodles received on this game lobby
    connect(newGameLobby,&GameLobbyHandler::allDoodlesReceived,this,&GameManager::allDoodlesInGameLobbyReceived);
    //Connection when all drawings received on this game lobby
    connect(newGameLobby,&GameLobbyHandler::allDrawingsReceived,this,&GameManager::allDrawingsInGameLobbyReceived);
    //Connection when votes are done winner is chosen
    connect(newGameLobby,&GameLobbyHandler::winnerChosen,this,&GameManager::winnerChosen);
    newGameLobby->addClient(uniqueID);
    m_gameLobbyMap[newLobbyID] = newGameLobby;
     qDebug()<< "New Game Lobby Created  : "<< newLobbyID;
     //sending gameLobby Id back to client
     m_socketHandler->sendTextMessageToClient("type:newLobbyCreated;payLoad:"+newLobbyID+";clientList:"+newGameLobby->clientsInLobby(),uniqueID) ;
}

void GameManager::joinGameLobbyRequest(QString lobbyID, QString uniqueID)
{
    if(m_gameLobbyMap.contains(lobbyID)){
        GameLobbyHandler* existingLobby = m_gameLobbyMap[lobbyID];
        existingLobby->addClient(uniqueID);
        m_socketHandler->sendTextMessageToMultipleClients("type:updatedClientList;payLoad:"+existingLobby->clientsInLobby(),existingLobby->clientsInLobbyList());
        m_socketHandler->sendTextMessageToClient("type:joinSuccess;payLoad:"+lobbyID+";clientList:"+existingLobby->clientsInLobby(),uniqueID);
    }
    else
    {
        m_socketHandler->sendTextMessageToClient("type:joinError;payLoad:DNE",uniqueID); //DoesNotExist
    }
}

void GameManager::messageLobbyRequest(QString message, QString lobbyID, QString senderID)
{
    if(m_gameLobbyMap.contains(lobbyID)){
        GameLobbyHandler * existingLobby = m_gameLobbyMap[lobbyID];
        m_socketHandler->sendTextMessageToMultipleClients("type:lobbyMessage;payLoad:"+message+";sender:"+senderID,existingLobby->clientsInLobbyList());
    }
}

void GameManager::userReadyListChanged()
{
    //debug as sender() is sending QObject
    GameLobbyHandler * existingLobby = qobject_cast<GameLobbyHandler*>(sender()); //the lobby that triggered the signal
    m_socketHandler->sendTextMessageToMultipleClients("type:readyListChanged;payLoad:"+existingLobby->whoIsReady(),existingLobby->clientsInLobbyList());

}

void GameManager::userReadyToPlay(QString uniqueID)
{
    qDebug() << "User ready : "<<uniqueID;
    QList<GameLobbyHandler*>  gameLobbyList  = m_gameLobbyMap.values();
    foreach (GameLobbyHandler* existingLobby, gameLobbyList) {
        existingLobby->userReadyToPlay(uniqueID); //will check for each lobby , if it contains the client set it to True or else ignore
    }

}

void GameManager::gameReadyToBegin()
{
    GameLobbyHandler * existingLobby = qobject_cast<GameLobbyHandler*>(sender()); //the lobby that triggered the signal
    //type:gameReadyToBegin;payLoad:0
    m_socketHandler->sendTextMessageToMultipleClients("type:gameReadyToBegin;payLoad:0",existingLobby->clientsInLobbyList());
}

void GameManager::newDoodleDataReady(QString fileData, QString clientID)
{
    //will check for each lobby , if it contains the client set its doodle otherwise ignore
    QList<GameLobbyHandler*>  gameLobbyList  = m_gameLobbyMap.values();
    foreach (GameLobbyHandler* existingLobby, gameLobbyList) {
        existingLobby->newDoodleData(fileData,clientID);
    }
}

void GameManager::newDoodleDrawReady(QString fileData, QString clientID)
{
    //game lobby handler to accept drawings
    //will check for each lobby , if it contains the client set its drawing otherwise ignore
    QList<GameLobbyHandler*>  gameLobbyList  = m_gameLobbyMap.values();
    foreach (GameLobbyHandler* existingLobby, gameLobbyList) {
        existingLobby->newDrawData(fileData,clientID);
    }

}

void GameManager::allDoodlesInGameLobbyReceived(QMap<QString,QString> distributedMap)
{
    //TODO
    GameLobbyHandler * existingLobby = qobject_cast<GameLobbyHandler*>(sender()); //the lobby that triggered the signal
    //type:drawingInstruction;payLoad:doodleData;instruction:dog
    //distributedmap  key:clientid val:newdoodle
    foreach(const QString& client,distributedMap.keys()){
        QString dataPacket = "type:drawingInstruction;payLoad:"+distributedMap[client]+";instruction:"+existingLobby->drawInstruction();
        m_socketHandler->sendTextMessageToClient(dataPacket,client);
    }

}

void GameManager::allDrawingsInGameLobbyReceived(QMap<QString, QString> drawingMap)
{
    //type:gameImages;payLoad:fileData1;fileData2,fileData3,;clients:1111,2222,3333
    GameLobbyHandler * existingLobby = qobject_cast<GameLobbyHandler*>(sender());
    QString ret = "type:gameImages;payLoad:";
    QString payLoad = QString();
    QString clients = QString();

    for(QMap<QString,QString>::iterator i = drawingMap.begin();i!=drawingMap.end();i++){
        clients.append(i.key() + ",");
        payLoad.append(i.value()+",");

    }
    clients.chop(1);
    payLoad.chop(1);
    ret.append(payLoad + ";clients:" + clients);
    m_socketHandler->sendTextMessageToMultipleClients(ret,existingLobby->clientsInLobbyList());
}

void GameManager::newVote(QString vote, QString senderID)
{
    QList<GameLobbyHandler*>  gameLobbyList  = m_gameLobbyMap.values();
    foreach (GameLobbyHandler* existingLobby, gameLobbyList) {
        existingLobby->newVote(vote,senderID);
    }
}

void GameManager::winnerChosen(QString winner)
{
    GameLobbyHandler * existingLobby = qobject_cast<GameLobbyHandler*>(sender());
    //type:winnerChosen;payLoad:winner
    m_socketHandler->sendTextMessageToMultipleClients("type:winnerChosen;payLoad:"+winner,existingLobby->clientsInLobbyList());
}
