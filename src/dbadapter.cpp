#include "dbadapter.h"

#include <QDebug>
#include <QDir>
#include <QSqlQueryModel>
#include <QtSql>

static dbAdapter* dbAdapterInstance = 0;

dbAdapter::dbAdapter(QObject* parent)
    : QObject(parent)
{
    QMutexLocker locker(&lock);
    QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    cacheDir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    loadDB();
}

dbAdapter::~dbAdapter()
{
    m_db.close();
}

dbAdapter& dbAdapter::instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (!dbAdapterInstance) {
        dbAdapterInstance = new dbAdapter();
    }
    return *dbAdapterInstance;
}

QSqlDatabase dbAdapter::getDatabase()
{
    return m_db;
}

void dbAdapter::initDB(QSqlDatabase db)
{
    db.exec("CREATE TABLE `weather` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, \
            `cityName` TEXT NOT NULL, \
            `cityID` INTEGER NOT NULL, \
            `dateTime` INTEGER NOT NULL, \
            `weatherIcon` TEXT NOT NULL, \
            `weatherDescription` TEXT NOT NULL,\
            `temperatureMin` NUMERIC NOT NULL,\
            `temperatureMax` NUMERIC NOT NULL,\
            `windSpeed` NUMERIC NOT NULL,\
            `windGusts` INTEGER NOT NULL,\
            `windDirection` INTEGER NOT NULL)");

    db.exec("CREATE TABLE `cityes` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, \
            `cityName` TEXT NOT NULL, \
            `cityID` INTEGER NOT NULL)");

    emit baseCreate();
}

void dbAdapter::loadDB()
{
    const QString dbPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/db.sql";
    const QString threadAddress = QLatin1String("0x") + QString::number((quintptr)QThread::currentThreadId(), 16);

    QSqlDatabase db = QSqlDatabase::database(threadAddress);

    if (db.isOpen() && db.isValid()) {
        return;
    } else {
        db = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), threadAddress);
    }
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << db.lastError().text();
    }

    qDebug() << "Load DB from " << dbPath;

    if (!QFile(dbPath).exists() || QFile(dbPath).size() == 0) {
        qDebug() << "Init db";
        initDB(db);
    }

    if (!db.isValid() || !db.isOpen()) {
        qFatal("Can't load DB");
    }

    m_db = db;
}
