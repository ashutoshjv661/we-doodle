#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "websockethandler.h"
#include "messageprocessorhandler.h" //to send messages from client to process in message handler
#include "gamelobbyhandler.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    ~GameManager();

signals:

public slots:
    void createGameLobbyRequest(QString uniqueID);
    void joinGameLobbyRequest(QString lobbyID,QString uniqueID);
    void messageLobbyRequest(QString message,QString lobbyID,QString senderID);
    void userReadyListChanged();
    void userReadyToPlay(QString uniqueID);
    void gameReadyToBegin();
    void newDoodleDataReady(QString fileData,QString clientID);
    void newDoodleDrawReady(QString fileData,QString clientID);
    void allDoodlesInGameLobbyReceived(QMap<QString,QString> distributedMap);
    void allDrawingsInGameLobbyReceived(QMap<QString,QString> drawingMap);
    void newVote(QString vote, QString senderID);
    void winnerChosen(QString winner);
private :
    WebSocketHandler * m_socketHandler;
    MessageProcessorHandler* m_messageProcessHandler;
    //to create multiple Game Lobby's and map them <string to gameLobby>
    QMap<QString,GameLobbyHandler *> m_gameLobbyMap;
};

#endif // GAMEMANAGER_H
