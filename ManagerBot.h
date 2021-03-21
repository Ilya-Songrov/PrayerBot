#pragma once

#include <QObject>
#include <QString>
#include <QVariant>

#include "FileWorker.h"
#include "GlobalData/GlobalData.h"
#include "Content/Content.h"
#include "Content/ChatInfo.h"
#include "BotPlaces/PlaceThyCloset/PlaceThyCloset.h"
#include "BotPlaces/PlaceAdditional/PlaceAdditional.h"

#include <tgbot/tgbot.h>
using namespace TgBot;

class ManagerBot : public QObject
{
    Q_OBJECT

public:
    explicit ManagerBot(const QString token = QString(), QObject *parent = nullptr);

    void startBot();

private:
    void setSettings();
    void anyMessageWasWrite(const Message::Ptr message);
    void callbackQueryWasWrite(const CallbackQuery::Ptr callbackQuery);

    ChatInfo getChatInfo(const std::int64_t chat_id, const std::string &currentText);
    void changeObjPtrPlaceBot(const Content::Place place);
    QString getTokenFromFile();

    void printChatInfo(const QString &header, const ChatInfo &chatInfo, const std::string &messageText);

private:
    PlaceThyCloset      *placeThyCloset ;
    PlaceAdditional     *placeAdditional;
    PlaceAbstract       *placeBot       ;
    std::shared_ptr<QMap<std::uint64_t, ChatInfo> > mapAllChats;
};

