#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>

#include "Controllers/websockethandler.h"
#include "Controllers/gamemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    WebSocketHandler socketHandler;
    socketHandler.connectToServer("ws://127.0.0.1:8585");

    GameManager gameManager;
    //To send messages from websockethandler to gamemanager
    QObject::connect(&socketHandler,&WebSocketHandler::newMessageReadyForProcessing,&gameManager,&GameManager::processSocketMessage);
    //To send message from gameManager to send message to socket
    QObject::connect(&gameManager,&GameManager::newMessageReadyToSend,&socketHandler,&WebSocketHandler::sendMessageToServer);


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    //To use C++ objects inside QML
    QQmlContext * context = engine.rootContext(); //Setting root context as we want socket throughout the life of program
    context->setContextProperty("webSocketHandler", &socketHandler);
    context->setContextProperty("gameManager", &gameManager);

    engine.load(url);
    return app.exec();
}
