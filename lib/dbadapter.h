#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QObject>
#include <QMutex>
#include <QSqlQuery>

class QSqlQueryModel;
class QSqlDatabase;

class dbAdapter : public QObject {
    Q_OBJECT
public:
    explicit dbAdapter(QObject* parent = 0);
    ~dbAdapter();

    static dbAdapter& instance();
    QSqlQueryModel* getTable(QString table);
    QSqlDatabase getDatabase();

signals:
    void baseCreate();

public slots:

private:
    QSqlQuery query;
    QMutex lock;
    void initDB(QSqlDatabase db);
    void loadDB();
    QSqlDatabase m_db;
};

#endif // DBADAPTER_H
