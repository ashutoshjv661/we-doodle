#ifndef MESSAGEPROCESSHANDLER_H
#define MESSAGEPROCESSHANDLER_H

#include <QObject>

class MessageProcessHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageProcessHandler(QObject *parent = nullptr);
    void processMessage(QString message);
signals:
    void uniqueIDRegistration(QString uniqueID);
    void newLobby(QString lobbyID, QStringList clients);
    void lobbyListUpdated(QStringList clients);
    void newLobbyMessage(QString message);
    void readyListChanged(QStringList readyList);
    void gameStarting();
    void doodleAndInstructionReady(QString payLoad,QString instruction);
    void gameDrawingsReady(QStringList images,QStringList clients);
    void winnerChosen(QString winner);
};

#endif // MESSAGEPROCESSHANDLER_H
