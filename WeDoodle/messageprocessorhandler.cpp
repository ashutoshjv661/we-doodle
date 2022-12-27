#include "messageprocessorhandler.h"
#include <QDebug>
#include <QRegularExpression>
#include <QString>
MessageProcessorHandler::MessageProcessorHandler(QObject *parent)
    : QObject{parent}
{

}

void MessageProcessorHandler::processMessage(QString message)
{
    //We will process messages as below
    //type:createGame;payLoad:0;sender:5555
    //type:joinGame;payLoad:4000;sender:5555;
    //type:message;payLoad:HelloWorld;lobbyID:8888;sender:5555;
    //type:readyToPlay;payLaod:1;sender:5555
    //type:doodleData;payLoad:fileData;sender:clientID
    //type:drawData;payLoad:fileData;sender:clientID
     //type:casteVote;payLoad:clientID;sender:1111
    qDebug() << "Server App : Message to process: " << message;

    auto re = new QRegularExpression(";");
    QStringList seperated = message.split(re->pattern());

    if(seperated.first() == "type:createGame"){
        //create game request
        qDebug() << "Create game request";
        seperated.pop_front(); //removing type and payload
        seperated.pop_front();
        if(seperated.first().contains("sender:")){
            QString senderID = seperated.front();
            senderID = senderID.remove("sender:");
            emit createGameRequest(senderID);
        }
    }else if(seperated.first() == "type:joinGame"){

        //Join Game
        qDebug() << "Join Game Request";
        seperated.pop_front();
        QString lobbyID = QString();
        QString senderID = QString();
        if(seperated.front().contains("payLoad:")){
            lobbyID = seperated.front();
            lobbyID = lobbyID.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.front().contains("sender:")){
                senderID = seperated.first();
                senderID.remove("sender:");
        }
        if(lobbyID!=QString() and senderID!=QString()){
            emit joinGameLobbyRequest(lobbyID,senderID);
        }


    }else if(seperated.first() == "type:message"){
        //message to lobby
        //type:message;payLoad:HelloWorld;lobbyID:8888;sender:5555;
        qDebug() << "lobby message request";
        QString payLoad = QString();
        QString lobbyID = QString();
        QString senderID = QString();
        seperated.pop_front();
        if(seperated.front().contains("payLoad:")){
            payLoad = seperated.front();
            payLoad = payLoad.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.front().contains("lobbyID:")){
            lobbyID = seperated.front();
            lobbyID = lobbyID.remove("lobbyID:");
        }
        seperated.pop_front();
        if(seperated.front().contains("sender:")){
            senderID = seperated.front();
            senderID = senderID.remove("sender:");
        }
        if(payLoad != QString() and lobbyID != QString() and senderID != QString()){
            emit messageLobbyRequest(payLoad,lobbyID,senderID);
        }
    }else if(seperated.front() == "type:readyToPlay"){
        //type:readyToPlay;payLaod:1;sender:5555
        if(seperated.back().contains("sender:")){
            QString clientID = seperated.back();
            clientID = clientID.remove("sender:");
            emit clientReadyToPlay(clientID);
        }
    }else if(seperated.front() == "type:doodleData" or seperated.front() == "type:drawData"){
        //type:doodleData;payLoad:fileData;sender:clientID
        //type:drawData;payLoad:fileData;sender:clientID
        QString fileData = QString();
        QString clientID = QString();
        QString type = seperated.front();
        type = type.remove("type:");
        seperated.pop_front();
        if(seperated.front().contains("payLoad:")){
            fileData = seperated.front();
            fileData = fileData.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.front().contains("sender:")){
            clientID = seperated.front();
            clientID = clientID.remove("sender:");
        }
        if(fileData!=QString() and clientID!=QString()  and type == "doodleData"){
            emit newDoodleData(fileData,clientID);
        }else if(fileData!=QString() and clientID!=QString()  and type == "drawData"){
            emit newDrawData(fileData,clientID);
        }

    }else if(seperated.front() == "type:casteVote"){
         //type:casteVote;payLoad:clientID;sender:1111
        QString vote = QString();
        QString senderID = QString();
        seperated.pop_front();
        if(seperated.front().contains("payLoad:")){
            vote = seperated.front();
            vote = vote.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.front().contains("sender:")){
            senderID = seperated.front();
            senderID = senderID.remove("sender:");
        }
        if(vote!=QString() and senderID!=QString()){
            emit newVote(vote,senderID);
        }
    }
}
