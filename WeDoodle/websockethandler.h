#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QMap>
class WebSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketHandler(QObject *parent = nullptr);
    ~WebSocketHandler();
    void sendTextMessageToClient(QString message,QString clientID);
    void sendTextMessageToMultipleClients(QString message,QStringList clientIDs);

signals:
    void newMessageToProcess(QString message);

public slots:
    void onNewSocketConnection();
    void onTextMessageReceived( QString );
    void onSocketDisconnected();
private:
    QWebSocketServer * m_socketServer;
    QMap< QString, QWebSocket*> m_clientList; //To store all connected clients
    //Storing a map of all clients to uniquely identify them and map them to Id's(String)
};

#endif // WEBSOCKETHANDLER_H
