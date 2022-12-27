#include "websockethandler.h"
#include <QDebug>

WebSocketHandler::WebSocketHandler(QObject *parent)
    : QObject{parent}
{
    m_webSocket = new QWebSocket;
    connect(m_webSocket,&QWebSocket::connected,this,&WebSocketHandler::onConnected);
    connect(m_webSocket,&QWebSocket::textMessageReceived,this,&WebSocketHandler::onTextMessageReceived);
}

WebSocketHandler::~WebSocketHandler(){
    m_webSocket->deleteLater();
}

void WebSocketHandler::connectToServer(QString hostAddress)
{
    qDebug() <<"Client App : connecting to server -> "<<hostAddress;
    m_webSocket->open(hostAddress); //Client will open the Websocket for the host Address provided
}

void WebSocketHandler::onConnected()
{
    qDebug() << "Client app connected!";
    //m_webSocket->sendTextMessage("type:createGame;payLoad:0");
}

void WebSocketHandler::onTextMessageReceived(QString message ){
    qDebug() << "Client app received message :"<<message;
    emit newMessageReadyForProcessing(message);
}

void WebSocketHandler::sendMessageToServer(QString message)
{
    m_webSocket->sendTextMessage(message);
}

