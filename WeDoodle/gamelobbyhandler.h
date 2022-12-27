#ifndef GAMELOBBYHANDLER_H
#define GAMELOBBYHANDLER_H

#include <QObject>
#include <QMap>

class GameLobbyHandler : public QObject
{
    Q_OBJECT
public:
    explicit GameLobbyHandler(QString lobbyID,QObject *parent); //explicit ensure not implicit type convertions
    void addClient(QString clientID);
    QString clientsInLobby();
    QStringList clientsInLobbyList();
    void userReadyToPlay(QString clientID);
    QString whoIsReady();
    void newDoodleData(QString fileData,QString clientID);
    void newDrawData(QString fileData,QString clientID);
    void newVote(QString vote,QString clientID);
    void chooseObject();
    QString drawInstruction();
signals:
    void userReadyListChanged(); //when we make a client ready we emit this to gamemanager
    void gameReadyToBegin();
    void allDoodlesReceived(QMap<QString,QString> newMap);
    void allDrawingsReceived(QMap<QString,QString> newMap);
    void winnerChosen(QString winner);

private:
    QStringList m_objectsToDraw;
    QString m_selectedObject;
    QString m_lobbyID;
    QList<QString> m_gameClientList; //Manage clients in particular game lobby
    QMap<QString ,bool> m_clientReadyList;
    QMap<QString,QString> m_clientDoodleData ; //key:clientId val:their doodles
    QMap<QString,QString> m_clientDrawData ;
    QList<QString> m_clientsWhoVoted;
    QMap<QString,int> m_voteResults;
};

#endif // GAMELOBBYHANDLER_H
