#include "messageprocesshandler.h"
#include <QDebug>
#include <QRegularExpression>
MessageProcessHandler::MessageProcessHandler(QObject *parent)
    : QObject{parent}
{

}

void MessageProcessHandler::processMessage(QString message)
{
    //type:uniqueID;payLoad:5555
    //type:newLobbyCreated;payLoad:9999;clientList:111,222,333
    //type:joinSuccess;payLoad:9999;;clientList:111,222,333
    //type:updatedClientList;payLoad:111,333,444
    //type:lobbyMessage;payLoad:"+message+";sender:"+senderID
    //type:readyListChanged;payLoad:"+existingLobby->whoIsReady()
    //type:gameReadyToBegin;payLoad:0
    //type:drawingInstruction;payLoad:doodleData;instruction:dog
    //type:gameImages;payLoad:fileData1;fileData2,fileData3,;clients:1111,2222,3333
    //type:winnerChosen;payLoad:winner
    auto re = new QRegularExpression(";");
    QStringList seperated = message.split(re->pattern());
    if(seperated.first() == "type:uniqueID"){
        //uniqueID registration
        qDebug() << "Client App uniqueID Registration";
        seperated.pop_front(); //to remove type
        //Incase payLoad is not sent or garbage is sent
        if(seperated.first().contains("payLoad:")){
            QString newClientID = seperated.first();
            newClientID = newClientID.remove("payLoad:");
            emit uniqueIDRegistration(newClientID);
        }
    }else if(seperated.first() == "type:newLobbyCreated" || seperated.first()== "type:joinSuccess"){
        qDebug() << "Client App: client joined new lobby ";
        seperated.pop_front();
        QString newLobbyID = QString();
        QStringList lobbyClients = QStringList();
        if(seperated.front().contains("payLoad:")){
            newLobbyID = seperated.first();
            newLobbyID = newLobbyID.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.first().contains("clientList:")){
            QString clients = seperated.first();
            clients = clients.remove("clientList:");
            auto re = new QRegularExpression(",");
            lobbyClients = clients.split(re->pattern());
        }

        qDebug() << "Client App: Clients in lobby: "<<lobbyClients;
        if(newLobbyID != QString() and lobbyClients != QStringList()){
            emit newLobby(newLobbyID,lobbyClients);
        }
    }else if(seperated.first() == "type:updatedClientList"){
        qDebug() << "Client App: received updated client list ";
        seperated.pop_front();
        QString payLoad = seperated.front();
        payLoad.remove("payLoad:");
        auto re = new QRegularExpression(",");
        emit lobbyListUpdated(payLoad.split(re->pattern()));
    }else if (seperated.first()=="type:lobbyMessage"){ //type:lobbyMessage;payLoad:"+message+";sender:"+senderID
        QString newMessage;
        QString senderID;
        seperated.pop_front();
        if(seperated.first().contains("payLoad:")){
            newMessage = seperated.first();
            newMessage = newMessage.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.first().contains("sender:")){
            senderID = seperated.first();
            senderID = senderID.remove("sender:");
        }
        QString displayMessage(senderID +": "+newMessage);
        emit newLobbyMessage(displayMessage);

    }else if(seperated.first() == "type:readyListChanged"){
         //type:readyListChanged;payLoad:"+existingLobby->whoIsReady()
        seperated.pop_front();
        QString payLoad = seperated.front();
        payLoad = payLoad.remove("payLoad:");
        auto re = new QRegularExpression(",");
        QStringList readyClients = payLoad.split(re->pattern());
        emit readyListChanged(readyClients);
    }else if(seperated.first() == "type:gameReadyToBegin"){
        //type:gameReadyToBegin;payLoad:0
        emit gameStarting();
    }else if(seperated.first() == "type:drawingInstruction"){
        //type:drawingInstruction;payLoad:doodleData;instruction:dog
        qDebug() <<" here Client App : Received drawing instruction! ";
        QString payLoad ;
        QString drawingInstruction ;
        seperated.pop_front();
        if(seperated.first().contains("payLoad:")){
            payLoad = seperated.front();
            payLoad = payLoad.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.first().contains("instruction:")){
            drawingInstruction = seperated.front();
            drawingInstruction = drawingInstruction.remove("instruction:");
        }
        if(!payLoad.isEmpty() && !drawingInstruction.isEmpty()){
            emit doodleAndInstructionReady(payLoad,drawingInstruction);
        }
    }else if(seperated.first() == "type:gameImages"){
        //type:gameImages;payLoad:fileData1;fileData2,fileData3,;clients:1111,2222,3333
        seperated.pop_front();
        QString payLoad = QString();
        QString clients = QString();
        if(seperated.first().contains("payLoad:")){
            payLoad = seperated.front();
            payLoad = payLoad.remove("payLoad:");
        }
        seperated.pop_front();
        if(seperated.first().contains("clients:")){
            clients = seperated.first();
            clients.remove("clients:");
        }
        if(payLoad != QString() and clients!=QString()){
            auto re = new QRegularExpression(",");
            emit gameDrawingsReady(payLoad.split(re->pattern()),clients.split(re->pattern()));
        }


    }else if(seperated.front() == "type:winnerChosen"){
        QString winner = QString();
        seperated.pop_front();
        if(seperated.front().contains("payLoad:")){
            winner = seperated.front();
            winner = winner.remove("payLoad:");
        }
        if(winner != QString()){
            emit winnerChosen(winner);
        }

    }
}
