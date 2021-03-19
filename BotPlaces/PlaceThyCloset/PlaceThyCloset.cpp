#include "PlaceThyCloset.h"

PlaceThyCloset::PlaceThyCloset(QObject *parent) : PlaceAbstract(parent)
{

}

void PlaceThyCloset::slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command)
{
    switch (command) {
    case Content::ThyCloset_AddPrayerNeed:
        onAddPrayerNeed(messagePtr);
        break;
    case Content::ThyCloset_AddAnswerOfGod:
        onAddAnswerOfGod(messagePtr);
        break;
    case Content::ThyCloset_ListPrayerNeed:
        onListPrayerNeed(messagePtr);
        break;
    case Content::Additional_Additional:
        onListPrayerNeed(messagePtr);
        break;
    case Content::MultiPlace_AnyMessage:
        onAnyMessage(messagePtr);
        break;
    default:
        PlaceAbstract::slotOnCommand(messagePtr, command);
    }
    lastCommand.insert(messagePtr->chat->id, command);
}

void PlaceThyCloset::onAddPrayerNeed(const Message::Ptr &messagePtr)
{
    static const auto answer { QObject::tr("Write your prayer need:").toStdString() };
    bot->getApi().sendMessage(messagePtr->chat->id, answer);
}

void PlaceThyCloset::onAddAnswerOfGod(const Message::Ptr &messagePtr)
{
    static const auto answer { QObject::tr("Select the prayer need:").toStdString() };
    const auto listNeedsWithNeedId = managerDatabase->getListPrayerNeedsWithNeedId(messagePtr->chat->id);
    QList<QPair<QString, QString> > listButtons;
    for (const auto &pair: listNeedsWithNeedId) {
        listButtons.append(qMakePair(pair.first.left(5), pair.second));
        qDebug() << "first:" << pair.first << "second:" << pair.second << Qt::endl;
    }
//    const auto inlineButtonPrayerNeed = createOneColumnInlineKeyboardMarkup(listButtons);
//    sendInlineKeyboardMarkupMessage(messagePtr->chat->id, answer, inlineButtonPrayerNeed);
}

void PlaceThyCloset::onListPrayerNeed(const Message::Ptr &messagePtr)
{
    bot->getApi().sendMessage(messagePtr->chat->id, getListPrayerNeeds(messagePtr), false, 0, getStartingButtons());
}

std::string PlaceThyCloset::getListPrayerNeeds(const Message::Ptr &messagePtr)
{
    const QString answer { "List prayers:\n" + managerDatabase->getListPrayerNeeds(messagePtr->chat->id).join('\n') };
    return answer.toStdString();
}

void PlaceThyCloset::onAnyMessage(const Message::Ptr &messagePtr)
{
    qDebug() << "lastCommand" << lastCommand << Qt::endl;
    if (containsLastCommand(messagePtr, Content::ThyCloset_AddPrayerNeed)) {
//        static const auto answer { QObject::tr("Your prayer need is added.").toStdString() };
        managerDatabase->addPrayerNeed(messagePtr->text, messagePtr->chat->id);
        sendStartingMessage(messagePtr->chat->id, getListPrayerNeeds(messagePtr));
    }
    else if (containsLastCommand(messagePtr, Content::ThyCloset_AddAnswerOfGod)) {
        static const auto answer { QObject::tr("Thank God!").toStdString() };
//        managerDatabase->deletePrayerNeed(messagePtr->text, messagePtr->chat->id);
//        sendStartingMessage(messagePtr->chat->id, answer);
    }
    else if (QString::fromStdString(messagePtr->text).toLower() == "ping") {
        sendStartingMessage(messagePtr->chat->id, "Pong!");
    }
}
