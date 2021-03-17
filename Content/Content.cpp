#include "Content.h"

#include <QMetaEnum>
#include <QPair>

QVector<QPair<Content::PlaceCommand, QString> > Content::vecPlaceCommand{
    {   Content::PlaceCommand(Content::Place::ThyCloset, Content::ThyCloset_AddPrayerNeed), QObject::tr("🙏 Add prayer need") },
    {   Content::PlaceCommand(Content::Place::ThyCloset, Content::ThyCloset_AddAnswerOfGod), QObject::tr("✔️ Add answer of God") },
    {   Content::PlaceCommand(Content::Place::ThyCloset, Content::ThyCloset_ListPrayerNeed), QObject::tr("🗒 List of prayer needs") },

    {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_Additional), QObject::tr("🔹 Additional") },
    {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_ShowHistory), QObject::tr("Show history 📍") },
    {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_DeletePrayerNeed), QObject::tr("Delete prayer need 📍") },
    {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_DeleteHistory), QObject::tr("Delete history 📍") },
    {   Content::PlaceCommand(Content::Place::Additional, Content::Additional_Developer), QObject::tr("Developer 📍") },

    {   Content::PlaceCommand(Content::Place::MultiPlace, Content::MultiPlace_Help), "/start" },
    {   Content::PlaceCommand(Content::Place::MultiPlace, Content::MultiPlace_Help), "/help" },
};

Content::Content(QObject *parent) : QObject(parent)
{

}

void Content::initContent()
{

}

Content::PlaceCommand Content::getPlaceCommand(const QString &command)
{
    for (const auto &pair: qAsConst(vecPlaceCommand)) {
        if (pair.second == command) {
            return pair.first;
        }
    }
    return {};
}

Content::PlaceCommand Content::getPlaceCommand(const std::string &command)
{
    return getPlaceCommand(QString::fromStdString(command));
}

QString Content::getCommandStr(const Content::Command placeCommand)
{
    if (placeCommand < vecPlaceCommand.size()) {
        return vecPlaceCommand.at(placeCommand).second;
    }
    return {};
}
