#pragma once

#include <QObject>
#include <QDebug>
#include <QtSql>

#include "../Content/Content.h"
#include "../GlobalData/GlobalData.h"

#include <tgbot/tgbot.h>
using namespace TgBot;

class PlaceAbstract : public QObject
{
    Q_OBJECT
public:
    explicit PlaceAbstract(QObject *parent = nullptr);

    virtual void slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command);
    virtual void slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const Content::Command &command);

protected:
    ReplyKeyboardMarkup::Ptr createOneColumnReplyKeyboardMarkup(const QStringList &listButtons, const bool resizeKeyboard = true, const bool oneTimeKeyboard = false);
    ReplyKeyboardMarkup::Ptr createReplyKeyboardMarkup(const QVector<QStringList> &vecLayouts, const bool resizeKeyboard = true, const bool oneTimeKeyboard = false);

    InlineKeyboardMarkup::Ptr createOneColumnInlineKeyboardMarkup(const QStringList &listButtons);
    InlineKeyboardMarkup::Ptr createInlineKeyboardMarkup(const QVector<QStringList> &vecLayouts);

    ReplyKeyboardMarkup::Ptr getStartingButtons();
    void sendStartingButtons(const std::int64_t id);
    void sendStartingMessage(const std::int64_t id, const std::string &message);

    inline bool containsLastCommand(const Message::Ptr &messagePtr, const Content::Command command){ return lastCommand.value(messagePtr->chat->id) == command; }
protected:
    QMap<std::uint64_t, Content::Command> lastCommand;
};

