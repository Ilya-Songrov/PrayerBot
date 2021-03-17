#pragma once

#include <QObject>
#include <QString>

#include "FileWorker.h"
#include "GlobalData/GlobalData.h"
#include "Content/Content.h"
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
    void anyMessageWasWrite(const Message::Ptr messagePtr);
    void callbackQueryWasWrite(const CallbackQuery::Ptr callbackQuery);

    void changePlaceBot(const Content::Place place);
    QString getTokenFromFile();

private:
    PlaceThyCloset      *placeThyCloset ;
    PlaceAdditional     *placeAdditional;
    PlaceAbstract       *placeBot       ;
};

