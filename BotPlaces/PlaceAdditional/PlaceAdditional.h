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
    void onShowHistory(const CallbackQuery::Ptr &callbackQuery);
    void onShowHistory(const Message::Ptr &messagePtr);
    void onDeletePrayerNeed(const Message::Ptr &messagePtr);
    void onDeleteHistory(const Message::Ptr &messagePtr);
    void onDeveloper(const Message::Ptr &messagePtr);
};

