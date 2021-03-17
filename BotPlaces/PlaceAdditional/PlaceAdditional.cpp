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
        //    case Content::Additional_ShowHistory:
        //        onShowHistory(messagePtr);
        //        break;
        //    case Content::Additional_DeletePrayerNeed:
        //        onDeletePrayerNeed(messagePtr);
        //        break;
        //    case Content::Additional_DeleteHistory:
        //        onDeleteHistory(messagePtr);
        //        break;
        //    case Content::Additional_Developer:
        //        onDeveloper(messagePtr);
        //        break;
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

void PlaceAdditional::onShowHistory(const CallbackQuery::Ptr &callbackQuery)
{
    const auto answer { QObject::tr("\n\nShow your history").toStdString() };
    bot->getApi().editMessageReplyMarkup(callbackQuery->message->chat->id, callbackQuery->message->messageId, "sdf", getStartingButtons());
//    sendStartingMessage(callbackQuery->message->chat->id, managerDatabase->getListPrayerNeeds(callbackQuery->message->chat->id).join("\n").toStdString());
//    bot->getApi().answerCallbackQuery(callbackQuery->id);
}

void PlaceAdditional::onShowHistory(const Message::Ptr &messagePtr)
{

}

void PlaceAdditional::onDeletePrayerNeed(const Message::Ptr &messagePtr)
{

}

void PlaceAdditional::onDeleteHistory(const Message::Ptr &messagePtr)
{

}

void PlaceAdditional::onDeveloper(const Message::Ptr &messagePtr)
{

}
