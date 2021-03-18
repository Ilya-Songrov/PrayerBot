#pragma once

#include <QObject>

#include "../PlaceAbstract.h"

class PlaceAdditional : public PlaceAbstract
{
public:
    explicit PlaceAdditional(QObject *parent = nullptr);

    virtual void slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command) override;
    virtual void slotOnCallbackQuery(const CallbackQuery::Ptr &callbackQuery, const Content::Command &command) override;

private:
    void onAdditional(const Message::Ptr &messagePtr);
    void onAnyMessage(const Message::Ptr &messagePtr);
    void onShowHistory(const CallbackQuery::Ptr &callbackQuery);
    void onDeletePrayerNeed(const CallbackQuery::Ptr &callbackQuery);
    void onDeleteHistory(const CallbackQuery::Ptr &callbackQuery);
    void onDeveloper(const CallbackQuery::Ptr &callbackQuery);

    void deletePrayerNeed(const Message::Ptr &messagePtr);
};

