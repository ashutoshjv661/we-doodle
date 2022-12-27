#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <Controllers/messageprocesshandler.h> //including message process handler inside Gamemanager
class GameManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString roomLobbyCode READ roomLobbyCode WRITE setRoomLobbyCode NOTIFY roomLobbyCodeChanged)
    Q_PROPERTY(QStringList clientsInLobby READ clientsInLobby WRITE setClientsInLobby NOTIFY clientsInLobbyChanged)
    Q_PROPERTY(QString drawingInstruction READ drawingInstruction WRITE setDrawingInstruction NOTIFY drawingInstructionChanged)
    Q_PROPERTY(QStringList drawingList READ drawingList WRITE setDrawingList NOTIFY drawingListChanged)
    Q_PROPERTY(QString winner READ winner WRITE setWinner NOTIFY winnerChanged)
public:
    explicit GameManager(QObject *parent = nullptr);
    ~GameManager();
    QString roomLobbyCode();
    QStringList clientsInLobby();
    QString drawingInstruction();
    QStringList drawingList();
    QString winner();

    Q_INVOKABLE void createGameRequest();
    Q_INVOKABLE void joinLobbyRequest(QString lobbyID);
    Q_INVOKABLE void sendMessageToLobby(QString message);
    Q_INVOKABLE bool isClientReady(QString clientID);
    Q_INVOKABLE void readyToPlay();
    Q_INVOKABLE void doodleDone();
    Q_INVOKABLE QString doodleFilePath();
    Q_INVOKABLE void casteVote(QString imageURL);
    Q_INVOKABLE QString winnerImage();
public slots:
    //Q_property
    void setRoomLobbyCode(QString lobbyCode);
    void setClientsInLobby(QStringList clientList);
    void setDrawingInstruction(QString instruction);
    void setDrawingList(QStringList newList);
    void setWinner(QString winner);


    void processSocketMessage(QString message);
    void registerUniqueID(QString uniqueID);
    void lobbyJoined(QString lobbyID,QStringList clients);
    void newClientReadyList(QStringList readyClients);
    void doodleAndInstructionReady(QString data,QString instruction);
    void gameDrawingsReady(QStringList images,QStringList clients);
    void winnerChosen(QString winner);
signals:
    //Q_property
    void roomLobbyCodeChanged();
    void clientsInLobbyChanged();
    void drawingInstructionChanged();
    void drawingListChanged();
    void winnerChanged();

    //gameManager signals
    void newMessageReadyToSend(QString message);
    void inGameLobby();
    void newLobbyMessage(QString message);
    void readyListChanged();
    void gameStarting();
    void drawInstructionTime();
    void votingTime();
    void gameOver();

private :
    bool m_isDrawing;
    bool m_voteFinised;
    QString m_clientID;
    QStringList m_clientsInLobby;
    QString m_drawingInstruction;
    QStringList m_drawingList;
    QString m_winner;

    QString m_roomLobbyCode;
    QStringList m_readyClientsList;
    MessageProcessHandler* m_messageHandler;
};

#endif // GAMEMANAGER_H
