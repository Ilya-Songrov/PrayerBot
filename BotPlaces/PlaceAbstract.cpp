#include "PlaceAbstract.h"

PlaceAbstract::PlaceAbstract(QObject *parent) : QObject(parent)
{

}

void PlaceAbstract::slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command)
{
    switch (command) {
    case Content::MultiPlace_Start:
        sendStartingButtons(messagePtr->chat->id);
        break;
    case Content::MultiPlace_Help:
        sendStartingButtons(messagePtr->chat->id);
        break;
    default:
        static const auto answer { QObject::tr("Query is not correct").toStdString() };
        sendStartingMessage(messagePtr->chat->id, answer);
    }
    lastCommand.insert(messagePtr->chat->id, command);
}

void PlaceAbstract::slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const Content::Command &command)
{
    switch (command) {
    case Content::MultiPlace_Start:
        sendStartingButtons(callbackQuery->message->chat->id);
        break;
    case Content::MultiPlace_Help:
        sendStartingButtons(callbackQuery->message->chat->id);
        break;
    default:
        static const auto answer { QObject::tr("Query is not correct").toStdString() };
        sendStartingMessage(callbackQuery->message->chat->id, answer);
    }
    lastCommand.insert(callbackQuery->message->chat->id, command);
}

ReplyKeyboardMarkup::Ptr PlaceAbstract::createOneColumnReplyKeyboardMarkup(const QStringList &listButtons, const bool resizeKeyboard, const bool oneTimeKeyboard)
{
    ReplyKeyboardMarkup::Ptr kb(new ReplyKeyboardMarkup);
    for (const QString &textButton : listButtons) {
        std::vector<KeyboardButton::Ptr> row;
        KeyboardButton::Ptr button(new KeyboardButton);
        button->text = textButton.toStdString();
        row.push_back(button);
        kb->keyboard.push_back(row);
    }
    kb->resizeKeyboard = resizeKeyboard;
    kb->oneTimeKeyboard = oneTimeKeyboard;
    return kb;
}

ReplyKeyboardMarkup::Ptr PlaceAbstract::createReplyKeyboardMarkup(const QVector<QStringList> &vecLayouts, const bool resizeKeyboard, const bool oneTimeKeyboard)
{
    ReplyKeyboardMarkup::Ptr kb(new ReplyKeyboardMarkup);
    for (int var = 0; var < vecLayouts.size(); ++var) {
        std::vector<KeyboardButton::Ptr> row;
        for (const QString &name: vecLayouts.at(var)) {
            KeyboardButton::Ptr button(new KeyboardButton);
            button->text = name.toStdString();
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
    kb->resizeKeyboard = resizeKeyboard;
    kb->oneTimeKeyboard = oneTimeKeyboard;
    return kb;
}

InlineKeyboardMarkup::Ptr PlaceAbstract::createOneColumnInlineKeyboardMarkup(const QStringList &listButtons)
{
    InlineKeyboardMarkup::Ptr kb(new InlineKeyboardMarkup);
    for (const QString &textButton : listButtons) {
        std::vector<InlineKeyboardButton::Ptr> row;
        InlineKeyboardButton::Ptr button(new InlineKeyboardButton);
        button->text = textButton.toStdString();
        button->callbackData = button->text;
        row.push_back(button);
        kb->inlineKeyboard.push_back(row);
    }
    return kb;
}

InlineKeyboardMarkup::Ptr PlaceAbstract::createInlineKeyboardMarkup(const QVector<QStringList> &vecLayouts)
{
    InlineKeyboardMarkup::Ptr kb(new InlineKeyboardMarkup);
    for (int var = 0; var < vecLayouts.size(); ++var) {
        std::vector<InlineKeyboardButton::Ptr> row;
        for (const QString &name: vecLayouts.at(var)) {
            InlineKeyboardButton::Ptr button(new InlineKeyboardButton);
            button->text = name.toStdString();
            button->callbackData = button->text;
            row.push_back(button);
        }
        kb->inlineKeyboard.push_back(row);
    }
    return kb;
}

ReplyKeyboardMarkup::Ptr PlaceAbstract::getStartingButtons()
{
    //    static const QStringList layout { Content::getCommand(Content::ThyCloset_AddPrayerNeed)
    //                                    , Content::getCommand(Content::ThyCloset_AddAnswerOfGod)
    //                                    , Content::getCommand(Content::ThyCloset_ListPrayerNeed) };
    //    static const auto kb = createOneColumnReplyKeyboardMarkup(layout, true, true);
    //    return kb;
    // /////////////////////////////////////////////////////// //
    //    static const auto kb = createReplyKeyboardMarkup({
    //                                                         {"🙏 Додати потребу", "❌ Видалити потребу"},
    //                                                         {"✔️ Додати відповідь від Бога"},
    //                                                         {"🗒 Поточні потреби", "🗒 Історія потреб"}
    //                                                     }, true, true);
    // /////////////////////////////////////////////////////// //
    static const QStringList layout { Content::getCommandStr(Content::ThyCloset_AddPrayerNeed)
                , Content::getCommandStr(Content::ThyCloset_AddAnswerOfGod)
                , Content::getCommandStr(Content::ThyCloset_ListPrayerNeed)
                , Content::getCommandStr(Content::Additional_Additional)
                                    };
    static const auto kb = createOneColumnReplyKeyboardMarkup(layout, true, true);
    return kb;
}

void PlaceAbstract::sendStartingButtons(const int64_t id)
{
    static const QString answer { QObject::tr("Hello child of God. This bot is designed to make your prayer life effective. \n\nMay God bless you.") };
    static const auto answerStdStr { answer.toStdString() };
    bot->getApi().sendMessage(id, answerStdStr, false, 0, getStartingButtons());
}

void PlaceAbstract::sendStartingMessage(const int64_t id, const std::string &message)
{
    bot->getApi().sendMessage(id, message, false, 0, getStartingButtons());
}
