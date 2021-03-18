#include "ManagerDatabase.h"

ManagerDatabase::ManagerDatabase(const QString &pathDatabase, QObject *parent) : QObject(parent)
{
    const bool needCreateTables = !QFileInfo::exists(pathDatabase);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(pathDatabase);
    if (db.open() && needCreateTables) {
        createDatabase();
    }
    else if(!db.open()){
        qWarning() << "Database Not open" << pathDatabase << Qt::endl;
    }
}

ManagerDatabase::~ManagerDatabase()
{
    if (db.isOpen()){
        db.close();
    }
}

bool ManagerDatabase::isOpen() const
{
    return db.isOpen();
}

bool ManagerDatabase::addPrayerNeed(const QString &prayerNeed, const int64_t chat_id)
{
    if (prayerNeed.isEmpty()){
        qWarning() << "addPrayerNeed failed: prayerNeed cannot be empty" << Qt::endl;
        return false;
    }
    if (existsPrayerNeed(prayerNeed, chat_id)) {
        return true;
    }
    if (!existsChatId(chat_id)) {
        inserNewChat(chat_id);
    }
    QSqlQuery query;
    query.prepare("INSERT INTO prayer_needs (need, chat_id) VALUES (:prayerNeed, :chat_id)");
    query.bindValue(":prayerNeed", prayerNeed);
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if(query.exec()){
        return true;
    }
    qWarning() << "addPrayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::addPrayerNeed(const std::string &prayerNeed, const int64_t chat_id)
{
    return addPrayerNeed(QString::fromStdString(prayerNeed), chat_id);
}

bool ManagerDatabase::deletePrayerNeed(const QString &prayerNeed, const int64_t chat_id)
{
    if (prayerNeed.isEmpty()){
        qWarning() << "deletePrayerNeed failed: prayerNeed cannot be empty" << Qt::endl;
        return false;
    }
    if (!existsPrayerNeed(prayerNeed, chat_id)) {
        return false;
    }
    QSqlQuery query;
    query.prepare("DELETE FROM prayer_needs WHERE (need = :prayerNeed AND chat_id = :chat_id)");
    query.bindValue(":chat_id", varinatChatId(chat_id));
    query.bindValue(":prayerNeed", prayerNeed);
    if(query.exec()){
        return true;
    }
    qWarning() << "deletePrayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::deletePrayerNeed(const std::string &prayerNeed, const int64_t chat_id)
{
    return deletePrayerNeed(QString::fromStdString(prayerNeed), chat_id);
}

bool ManagerDatabase::deleteAllPrayerNeeds(const int64_t chat_id)
{
    const bool retChat = deleteChat(chat_id);
    const bool retNeed = deletePrayerNeed(chat_id);
    return retChat && retNeed;
}

QStringList ManagerDatabase::getListPrayerNeeds(const int64_t chat_id)
{
    QStringList list;
    QSqlQuery query(QString("SELECT * FROM prayer_needs WHERE chat_id = %1").arg(chat_id));
    const QSqlRecord record = query.record();
    const int idNeed = record.indexOf("need");
    int step = 0;
    while (query.next())
    {
        const QString need = query.value(idNeed).toString();
        list.append(QString("%1. %2").arg(++step).arg(need));
    }
    return list;
}

void ManagerDatabase::printDatabase() const
{
    qDebug() << "printDatabase:" << Qt::endl;
    QSqlQuery query("SELECT * FROM prayer_needs");
    const QSqlRecord record = query.record();
    const int idNeed = record.indexOf("need");
    const int idChat_id = record.indexOf("chat_id");
    while (query.next())
    {
        const QString need = query.value(idNeed).toString();
        const int chat_id = query.value(idChat_id).toInt();
        qDebug() << QString("Need (%1) Chat_id (%2)").arg(need).arg(chat_id) << Qt::endl;
    }
}

void ManagerDatabase::createDatabase()
{
    createTable_AllChats();
    createTable_PrayerNeeds();
}

bool ManagerDatabase::createTable_AllChats()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE all_chats ("
            "chat_id INTEGER NOT NULL,"
            "PRIMARY KEY (chat_id),"
            "UNIQUE (chat_id)"
            ");");

    if (query.exec()){
        return true;
    }
    qWarning() << "Couldn't create the table 'all_chats': one might already exist." << Qt::endl;
    return false;
}

bool ManagerDatabase::createTable_PrayerNeeds()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE prayer_needs ("
            "need TEXT NOT NULL,"
            "chat_id INTEGER NOT NULL,"
            "FOREIGN KEY (chat_id) REFERENCES all_chats(chat_id) ON DELETE CASCADE"
            ");");

    if (query.exec()){
        return true;
    }
    qWarning() << "Couldn't create the table 'prayer_needs': one might already exist." << Qt::endl;
    return false;
}

bool ManagerDatabase::inserNewChat(const int64_t chat_id)
{
    QSqlQuery query;
    query.prepare("INSERT INTO all_chats (chat_id) VALUES (:chat_id)");
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if(query.exec()){
        return true;
    }
    qWarning() << "Add prayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::deleteChat(const int64_t chat_id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM all_chats WHERE (chat_id = :chat_id)");
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if(query.exec()){
        return true;
    }
    qWarning() << "deleteChat failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::deletePrayerNeed(const int64_t chat_id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM prayer_needs WHERE (chat_id = :chat_id)");
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if(query.exec()){
        return true;
    }
    qWarning() << "deletePrayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::existsChatId(const int64_t chat_id) const
{
    QSqlQuery query;
    query.prepare("SELECT chat_id FROM all_chats WHERE (chat_id = :chat_id)");
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if (query.exec()){
        return query.next();
    }
    qWarning() << "existsChatId failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::existsPrayerNeed(const QString &prayerNeed, const int64_t chat_id) const
{
    QSqlQuery query;
    query.prepare("SELECT * FROM prayer_needs WHERE (need = ':prayerNeed' AND chat_id = :chat_id);");
    query.bindValue(":prayerNeed", prayerNeed);
    query.bindValue(":chat_id", varinatChatId(chat_id));
    if (query.exec()){
        return query.next();
    }
    qWarning() << "existsPrayerNeed failed: " << query.lastError() << Qt::endl;
    return query.next();
}
