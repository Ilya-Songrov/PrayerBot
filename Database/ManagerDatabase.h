#pragma once

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>

class ManagerDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ManagerDatabase(const QString &pathDatabase, QObject *parent = nullptr);
    ~ManagerDatabase();

    bool isOpen() const;

    bool addPrayerNeed(const QString &prayerNeed, const std::int64_t chat_id);
    bool addPrayerNeed(const std::string &prayerNeed, const std::int64_t chat_id);

    bool removePrayerNeed(const QString &prayerNeed, const std::int64_t chat_id);
    bool removePrayerNeed(const std::string &prayerNeed, const std::int64_t chat_id);

    bool removeChatId(const std::int64_t chat_id);

    QStringList getListPrayerNeeds(const std::int64_t chat_id);
    void printDatabase() const;
private:
    void createDatabase();
    bool createTable_AllChats();
    bool createTable_PrayerNeeds();
    bool existsChatId(const std::int64_t chat_id) const;
    bool existsPrayerNeed(const QString &prayerNeed, const std::int64_t chat_id) const;

private:
    QSqlDatabase db;
};

