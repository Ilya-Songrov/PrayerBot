#pragma once

#include <QObject>

#include "../PlaceAbstract.h"

class PlaceThyCloset : public PlaceAbstract
{
public:
    explicit PlaceThyCloset(QObject *parent = nullptr);

    virtual void slotOnCommand(const Message::Ptr &messagePtr, const Content::Command &command) override;

private:
    void onAddPrayerNeed(const Message::Ptr &messagePtr);
    void onAddAnswerOfGod(const Message::Ptr &messagePtr);
    void onListPrayerNeed(const Message::Ptr &messagePtr);
    std::string getListPrayerNeeds(const Message::Ptr &messagePtr);
    void onAnyMessage(const Message::Ptr &messagePtr);
};

