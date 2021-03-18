#include "PlaceAdditional.h"

PlaceAdditional::PlaceAdditional(QObject *parent) : PlaceAbstract(parent)
{

}

void PlaceAdditional::slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command)
{
    switch (command) {
    case Content::Additional_Additional:
        onAdditional(messagePtr);
        break;
    case Content::MultiPlace_AnyMessage:
        onAdditional(messagePtr);
        break;
    default:
        PlaceAbstract::slotOnCommand(messagePtr, command);
    }
    lastCommand.insert(messagePtr->chat->id, command);
}

void PlaceAdditional::slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const Content::Command &command)
{
    switch (command) {
    case Content::Additional_ShowHistory:
        onShowHistory(callbackQuery);
        break;
    case Content::Additional_DeletePrayerNeed:
        onDeletePrayerNeed(callbackQuery);
        break;
    case Content::Additional_DeleteHistory:
        onDeleteHistory(callbackQuery);
        break;
    case Content::Additional_Developer:
        onDeveloper(callbackQuery);
        break;
    default:
        PlaceAbstract::slotOnCallbackQuery(callbackQuery, command);
    }
    lastCommand.insert(callbackQuery->message->chat->id, command);
}

void PlaceAdditional::onAdditional(const Message::Ptr &messagePtr)
{
    static const auto keyboard = createOneColumnInlineKeyboardMarkup({Content::getCommandStr(Content::Additional_ShowHistory)
                                                                      , Content::getCommandStr(Content::Additional_DeletePrayerNeed)
                                                                      , Content::getCommandStr(Content::Additional_DeleteHistory)
                                                                      , Content::getCommandStr(Content::Additional_Developer)
                                                                     });
    static const auto answer { QObject::tr("Your history:").toStdString() };
    bot->getApi().sendMessage(messagePtr->chat->id, answer, false, 0, keyboard, "Markdown");
}

void PlaceAdditional::onAnyMessage(const Message::Ptr &messagePtr)
{
    qDebug() << "lastCommand" << lastCommand << Qt::endl;
    if (containsLastCommand(messagePtr, Content::Additional_DeletePrayerNeed)) {
        deletePrayerNeed(messagePtr);
    }
    else if (containsLastCommand(messagePtr, Content::Additional_DeleteHistory)) {
        static const auto answer { QObject::tr("Your prayer need has deleted").toStdString() };
        managerDatabase->deletePrayerNeed(messagePtr->text, messagePtr->chat->id);
        sendStartingMessage(messagePtr->chat->id, answer);
    }
    else if (QString::fromStdString(messagePtr->text).toLower() == "ping") {
        sendStartingMessage(messagePtr->chat->id, "Pong!");
    }
}

void PlaceAdditional::onShowHistory(const CallbackQuery::Ptr &callbackQuery)
{
    const auto answer { QObject::tr("\n\nShow your history").toStdString() };
    //    bot->getApi().editMessageReplyMarkup(callbackQuery->message->chat->id, callbackQuery->message->messageId, "sdf", getStartingButtons());
    //    sendStartingMessage(callbackQuery->message->chat->id, managerDatabase->getListPrayerNeeds(callbackQuery->message->chat->id).join("\n").toStdString());
    bot->getApi().answerCallbackQuery(callbackQuery->id);
}

void PlaceAdditional::onDeletePrayerNeed(const CallbackQuery::Ptr &callbackQuery)
{
    static const auto enterNumber { QObject::tr("Enter number of prayer need:").toStdString() };
    const QString answer { "List prayers:\n" + managerDatabase->getListPrayerNeeds(callbackQuery->message->chat->id).join('\n') };
    bot->getApi().answerCallbackQuery(callbackQuery->id);
    bot->getApi().sendMessage(callbackQuery->message->chat->id, answer.toStdString() + "\n" + enterNumber);
}

void PlaceAdditional::onDeleteHistory(const CallbackQuery::Ptr &callbackQuery)
{

}

void PlaceAdditional::onDeveloper(const CallbackQuery::Ptr &callbackQuery)
{
    const auto answer { QObject::tr("Hello! I'm Ilya. Write me if you have any questions (https://t.me/ilyaSongrov)").toStdString() };
    sendStartingMessage(callbackQuery->message->chat->id, answer);
}

void PlaceAdditional::deletePrayerNeed(const Message::Ptr &messagePtr)
{
    const auto list = managerDatabase->getListPrayerNeeds(messagePtr->chat->id);
    const QString input = QString::fromStdString(messagePtr->text).trimmed();
    int step = 0;
    for (const QString &need: list) {
        ++step;
        if (need.left(input.size()) == input) {
            const QString number = QString::number(step);
            const QString deletedNeed = need.right(need.size() - number.size());
            managerDatabase->deletePrayerNeed(deletedNeed, messagePtr->chat->id);
            return;
        }
    }
}
