#include "websockethandler.h"
#include <QDebug> //To show debug statements
#include <random> //to generate random numbers

WebSocketHandler::WebSocketHandler(QObject *parent)
    : QObject{parent}
{
    m_socketServer = new QWebSocketServer("WeDoodleServer",QWebSocketServer::NonSecureMode,this); //creating  a websocket passing this as parent
    connect(m_socketServer,&QWebSocketServer::newConnection,this,&WebSocketHandler::onNewSocketConnection);
    // creates New connection and passes current class, on new connection the onNewSocketConnection method is invoked

    if(m_socketServer->listen(QHostAddress::Any,8585)){ //Listening any connection on PORT 8585
        qDebug() << "Server is running" ;
    }else{
        qDebug() << "Unable to Start listening for connections";
    }
}

WebSocketHandler::~WebSocketHandler(){
    m_socketServer->deleteLater();
}

void WebSocketHandler::sendTextMessageToClient(QString message, QString clientID)
{
    if(m_clientList.contains(clientID)){
        QWebSocket* existingClient = m_clientList[clientID];
        existingClient->sendTextMessage(message);
    }
}

void WebSocketHandler::sendTextMessageToMultipleClients(QString message, QStringList clientIDs)
{
    foreach(const QString& client,clientIDs){
        sendTextMessageToClient(message,client);
    }
}

void WebSocketHandler::onNewSocketConnection(){
    qDebug() << "New Client Connected !";

    std::random_device rd;
    std::default_random_engine generator; //pseudo random generator , after some time numbers are repeated
    generator.seed(rd()); //so we seed it with rd to generate unique value everytime
    std::uniform_int_distribution<int> idGenerator(1000,9999); //generate 4 digit unique value
    QString newClientID = QString::number(idGenerator(generator));

    //QMap<QString,QWebsocket*>
    //ensuring that ID is unique
    while(m_clientList.keys().contains(newClientID)){
        newClientID = QString::number(idGenerator(generator));
    }

    QWebSocket* nextClient = m_socketServer->nextPendingConnection();
    nextClient->setParent(this);
    connect(nextClient,&QWebSocket::textMessageReceived,this,&WebSocketHandler::onTextMessageReceived); //Signal to invoke slot if message received from client
    connect(nextClient,&QWebSocket::disconnected,this,&WebSocketHandler::onSocketDisconnected);

    //Sending generatedID back to client to know what is its Id
    nextClient->sendTextMessage("type:uniqueID;payLoad:"+newClientID);

    //Signal for socket disconnected, we can access the signal sender client  using sender()
    //m_clientList.append(nextClient); //Add client to the list -> We are using Map now
    m_clientList[newClientID] = nextClient;
}

void WebSocketHandler::onTextMessageReceived(QString message)
{
    qDebug() << "Server received :"<<message;
    emit newMessageToProcess(message);
}

void WebSocketHandler::onSocketDisconnected()
{
    QWebSocket* client = qobject_cast<QWebSocket*>(sender());  //QWebSocket object
    if(client){
        qDebug() <<"Disconnecting client "<<client;
        QMap<QString, QWebSocket*>::iterator i;
        for(i = m_clientList.begin();i!=m_clientList.end();i++){
            if(i.value() == client){
                QString uid = i.key();
                m_clientList.remove(uid);
                client->deleteLater(); //delete the sender later
            }
        }

    }
}
