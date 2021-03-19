#include "ManagerBot.h"


ManagerBot::ManagerBot(const QString token, QObject *parent) : QObject(parent)
{
    initGlobalData(token.isEmpty() ? getTokenFromFile() : token);

    placeThyCloset      = new PlaceThyCloset    (this);
    placeAdditional     = new PlaceAdditional   (this);
    placeBot            = placeThyCloset;
    setSettings();
}

void ManagerBot::startBot()
{
    //    Content::initContent();

//    signal(SIGINT, [](int s) {
//        printf("SIGINT got\n");
//        exit(0);
//    });

    qDebug("Token: %s\n", bot->getToken().c_str());
    try {
        qDebug("Bot username: %s\n", bot->getApi().getMe()->username.c_str());
        bot->getApi().deleteWebhook();

        TgLongPoll longPoll(*bot.get());
        while (true) {
            qDebug("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception& e) {
        qDebug("error: %s\n", e.what());
    }
}

void ManagerBot::setSettings()
{
    bot->getEvents().onAnyMessage(std::bind(&ManagerBot::anyMessageWasWrite, this, std::placeholders::_1));
    bot->getEvents().onCallbackQuery(std::bind(&ManagerBot::callbackQueryWasWrite, this, std::placeholders::_1));

    bot->getEvents().onInlineQuery([](const InlineQuery::Ptr){ qDebug() << "onInlineQuery" << Qt::endl; });
    bot->getEvents().onChosenInlineResult([](const ChosenInlineResult::Ptr){ qDebug() << "onChosenInlineResult" << Qt::endl; });
//    bot->getEvents().onCallbackQuery([](const CallbackQuery::Ptr &callbackQuery){ qDebug() << "onCallbackQuery" << callbackQuery->data.c_str() << Qt::endl; });
}

void ManagerBot::anyMessageWasWrite(const Message::Ptr messagePtr)
{
    const Content::PlaceCommand placeCommand = Content::getPlaceCommand(messagePtr->text);
    qDebug() << "anyMessageWasWrite:" << messagePtr->text.c_str() << placeCommand.place << placeCommand.command << Qt::endl;
    changePlaceBot(placeCommand.place);
    placeBot->slotOnCommand(messagePtr, placeCommand.command);
}

void ManagerBot::callbackQueryWasWrite(const CallbackQuery::Ptr callbackQuery)
{
    const Content::PlaceCommand placeCommand = Content::getPlaceCommand(callbackQuery->data);
    qDebug() << "callbackQueryWasWrite:" << callbackQuery->data.c_str() << placeCommand.place << placeCommand.command << Qt::endl;
    changePlaceBot(placeCommand.place);
    placeBot->slotOnCallbackQuery(callbackQuery, placeCommand.command);
}

void ManagerBot::changePlaceBot(const Content::Place place)
{
    switch (place) {
    case Content::Place::ThyCloset:
        placeBot = placeThyCloset;
        break;
    case Content::Place::Additional:
        placeBot = placeAdditional;
        break;
    case Content::Place::MultiPlace:
        break;
    default:
        break;
    }
}

QString ManagerBot::getTokenFromFile()
{
    const QJsonDocument doc = FileWorker::readFileJson("../config.json");
    const QJsonObject obj = doc.object();
    return obj.value("token").toString();
}
