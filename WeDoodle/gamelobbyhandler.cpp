#include "gamelobbyhandler.h"
#include <random>
GameLobbyHandler::GameLobbyHandler(QString lobbyID,QObject *parent)
    : QObject{parent}, m_lobbyID(lobbyID)
{
    m_clientReadyList.clear();
    m_clientDoodleData.clear();
    m_clientDrawData.clear();
    m_objectsToDraw = QStringList() <<"dog"<<"cat"<<"elephant"<<"car"<<"ball";
}

void GameLobbyHandler::addClient(QString clientID)
{
    //if clientID not present in list add it
    if(!m_gameClientList.contains(clientID)){
        m_gameClientList.append(clientID);
    }
    m_clientReadyList.clear(); //Whenever a new player joins all are back to not ready
    foreach (const QString &string, m_gameClientList) {
        m_clientReadyList[string] = false;
    }
    emit userReadyListChanged();
}

QString GameLobbyHandler::clientsInLobby()
{
    QString ret;
    foreach (const QString& client, m_gameClientList) {
        ret.append(client + ",");
    }
    ret.chop(1); //remove last ',' from back
    return ret;
}

QStringList GameLobbyHandler::clientsInLobbyList()
{
    QStringList ret;
    foreach (const QString& client, m_gameClientList) {
        ret.append(client);
    }
    return ret;
}

void GameLobbyHandler::userReadyToPlay(QString clientID)
{
    if(m_gameClientList.contains(clientID)){
        m_clientReadyList[clientID] = true;
        emit userReadyListChanged();

        bool notReady = false;
        foreach (const QString& clientID, m_clientReadyList.keys()) {
            if(!m_clientReadyList[clientID]){
                notReady = true;
            }
        }
        if(!notReady){
            emit gameReadyToBegin();
        }
    }

}

QString GameLobbyHandler::whoIsReady()
{
    QString returnVal = QString();
    //foreach(const QString& client,m_clientReadyList.keys())
    QStringList clientsInReadyList = m_clientReadyList.keys();
    for( int i=0;i<clientsInReadyList.size();i++){
        QString clientID = clientsInReadyList[i];
        if(m_clientReadyList[clientID]){
            returnVal.append(clientID+",");
        }
    }
    if(returnVal!=QString()){
        returnVal.chop(1);
    }
    return returnVal;
}

void GameLobbyHandler::newDoodleData(QString fileData, QString clientID)
{
    if(m_gameClientList.contains(clientID)){
        m_clientDoodleData[clientID] = fileData;
        //client size is same as clients sending doodle then we are ready for round2
        if(m_clientDoodleData.keys().size()==m_gameClientList.size()){
            //distribute doodle to other player
            //provide drawing instructions
            //key = clientID val = doodle (not his own but the clients before them )
            //As we are sending them back to vote for the best doodle
            QMap<QString,QString> distributedDoodles;
            for(int i=0;i<m_gameClientList.size();i++){
                QString currentClient = m_gameClientList[i]; //.at(i)
                QString doodle = QString();
                if(i == m_gameClientList.size()-1){
                    doodle = m_clientDoodleData[m_gameClientList.at(0)];
                }
                else {
                    doodle = m_clientDoodleData[m_gameClientList.at(i+1)];
                }
                distributedDoodles[currentClient] = doodle;

            }
            chooseObject();
            emit allDoodlesReceived(distributedDoodles);
        }
    }
}

void GameLobbyHandler::newDrawData(QString fileData, QString clientID)
{
    if(m_gameClientList.contains(clientID)){
        m_clientDrawData[clientID] = fileData;
        //client size is same as clients sending drawings then we are ready for round3
        if(m_clientDrawData.keys().size()==m_gameClientList.size()){
            emit allDrawingsReceived(m_clientDrawData );
        }
    }
}

void GameLobbyHandler::newVote(QString vote, QString clientID)
{
    if(m_gameClientList.contains(clientID) and !m_clientsWhoVoted.contains(clientID)){
        if(m_voteResults.contains(vote)){
            m_voteResults[vote]++;
        }else{
            m_voteResults[vote] = 1;
        }
        m_clientsWhoVoted.append(clientID);
        if(m_clientsWhoVoted.size() == m_gameClientList.size()){
            QString winner = QString();
            int highestVote = 0;
            QList<QString> clients = m_voteResults.keys();
            foreach (const QString & client, clients) {
                if(m_voteResults[client] > highestVote){
                    winner = client;
                    highestVote = m_voteResults[client];
                }
            }
            emit winnerChosen(winner);
        }
    }
}

void GameLobbyHandler::chooseObject(){
    std::random_device rd;
    std::default_random_engine generator; //pseudo random generator , after some time numbers are repeated
    generator.seed(rd()); //so we seed it with rd to generate unique value everytime
    std::uniform_int_distribution<int> objectGenerator(0,m_objectsToDraw.size()-1);
    m_selectedObject = m_objectsToDraw[objectGenerator(generator)];
}

QString GameLobbyHandler::drawInstruction()
{
    return m_selectedObject;
}
