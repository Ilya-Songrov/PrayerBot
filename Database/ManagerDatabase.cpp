#include "ManagerDatabase.h"

ManagerDatabase::ManagerDatabase(const QString &pathDatabase, QObject *parent) : QObject(parent)
{
#ifdef QT_DEBUG
//    QFile::remove(pathDatabase);
//    qFatal("remove");
#endif
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(pathDatabase);
    if (!db.open()) {
        qDebug() << "Database Not open" << pathDatabase << Qt::endl;
    }
#ifdef QT_DEBUG
//    createDatabase();
#endif
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
        qDebug() << "Add prayerNeed failed: prayerNeed cannot be empty" << Qt::endl;
        return false;
    }
    if (existsPrayerNeed(prayerNeed, chat_id)) {
        return true;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO prayer_needs (need, chat_id) VALUES (:prayerNeed, :chat_id)");
    query.bindValue(":chat_id", QVariant::fromValue(chat_id));
    query.bindValue(":prayerNeed", prayerNeed);
    if(query.exec()){
        return true;
    }
    qDebug() << "Add prayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::addPrayerNeed(const std::string &prayerNeed, const int64_t chat_id)
{
    return addPrayerNeed(QString::fromStdString(prayerNeed), chat_id);
}

bool ManagerDatabase::removePrayerNeed(const QString &prayerNeed, const int64_t chat_id)
{
    if (prayerNeed.isEmpty()){
        qDebug() << "Remove prayerNeed failed: prayerNeed cannot be empty" << Qt::endl;
        return false;
    }
    if (!existsPrayerNeed(prayerNeed, chat_id)) {
        return false;
    }
    qDebug() << "Remove" << prayerNeed << chat_id << Qt::endl;
    QSqlQuery query;
    query.prepare("DELETE FROM prayer_needs WHERE (need = :prayerNeed AND chat_id = :chat_id)");
    query.bindValue(":chat_id", QVariant::fromValue(chat_id));
    query.bindValue(":prayerNeed", prayerNeed);
    if(query.exec()){
        return true;
    }
    qDebug() << "Remove prayerNeed failed: " << query.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::removePrayerNeed(const std::string &prayerNeed, const int64_t chat_id)
{
    return removePrayerNeed(QString::fromStdString(prayerNeed), chat_id);
}

bool ManagerDatabase::removeChatId(const int64_t chat_id)
{

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
    qDebug() << "Couldn't create the table 'all_chats': one might already exist." << Qt::endl;
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
    qDebug() << "Couldn't create the table 'prayer_needs': one might already exist." << Qt::endl;
    return false;
}

bool ManagerDatabase::existsChatId(const int64_t chat_id) const
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT chat_id FROM all_chats WHERE chat_id = (:chat_id)");
    checkQuery.bindValue(":chat_id", QVariant::fromValue(chat_id));

    if (checkQuery.exec()){
        return checkQuery.next();
    }
    qDebug() << "Chat_id exists failed: " << checkQuery.lastError() << Qt::endl;
    return false;
}

bool ManagerDatabase::existsPrayerNeed(const QString &prayerNeed, const int64_t chat_id) const
{
    QSqlQuery query(QString("SELECT * FROM prayer_needs WHERE (need = '%1' AND chat_id = %2);").arg(prayerNeed).arg(chat_id));
    return query.next();
}
