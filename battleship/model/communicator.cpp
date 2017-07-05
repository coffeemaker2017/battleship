#include "communicator.h"
#include "game.h"
#include "connection_guest.h"
#include "connection_host.h"
#include <memory>
#include <QJsonObject>
#include <QJsonDocument>
#include "common/user_info.h"

MODEL::Communicator::Communicator(   MODEL::Game& game,
                                                            const std::string& address, 
                                                            int port,
                                                            std::function<void()> callbackOnConnected)
    : game{game}, conn{ std::make_unique<MODEL::ConnectionGuest>( address, port, 
        [&](const QByteArray& data) {dataReceived(data); },callbackOnConnected) } //TODO std::bind doesn't work ?
{
    //std::bind(&MODEL::Communicator::dataReceived, this)
}

MODEL::Communicator::Communicator(   MODEL::Game& game,
                                                            std::function<void()> callbackOnConnected)
    : game{game}, conn{ std::make_unique<MODEL::ConnectionHost>( 
        [&](const QByteArray& data) {dataReceived(data); },callbackOnConnected) } //TODO std::bind doesn't work ?
{
    //std::bind(&MODEL::Communicator::dataReceived, this)
}

void MODEL::Communicator::sendJson(const MODEL::Command& command, const QJsonObject& json)
{
//     QJsonObject resultJson{};
//     resultJson[QString::number(static_cast<int>(command))] = json;
    QJsonObject resultJson{
        {"command", static_cast<int>(command)},
        {"value", json}
    };
    
    QJsonDocument doc(resultJson);
    conn->sendData(doc.toJson());

    qDebug() << "Communicator::sendJson() -> command enum: " << static_cast<int>(command);
}


void MODEL::Communicator::dataReceived(const QByteArray& data)
{
    qDebug() << "COMM::dataReceived: " << data;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) { // hat es geklappt?
        throw std::runtime_error{"JSON-Format nicht erkannt"};
    }
    if (!doc.isObject()) {
        throw std::runtime_error{"QJsonDocument enthaelt mehrere Objekte"};
    }
    QJsonObject jsonTotal{ doc.object() };
//     int command{ jsonTotal["command"].toInt() };
    Command command{ static_cast<Command>(jsonTotal["command"].toInt()) };
//     qDebug() << "command enum as int: " << command;
    switch (command) {
        case Command::USER_INFO :
                qDebug() << "YES !!! Command::USER_INFO: ";
            break;
        case Command::SHIP_PLACEMENT :
            break;
    }
    
}




void MODEL::Communicator::sendUserInfo(const UserInfo& userInfo)
{
    QJsonObject json{
        {"name", QString::fromStdString(userInfo.getName())},
        {"age", userInfo.getAge()}
    };
    qDebug() << "Communicator::sendUserInfo: " << json;
    sendJson(MODEL::Command::USER_INFO, json);
}












