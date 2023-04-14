#include "weathermodel.h"

WeatherModel::WeatherModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_hash.insert(Qt::UserRole, QByteArray("cityName"));
    m_hash.insert(Qt::UserRole + 1, QByteArray("cityID"));
    m_hash.insert(Qt::UserRole + 2, QByteArray("dateTime"));
    m_hash.insert(Qt::UserRole + 3, QByteArray("weatherIcon"));
    m_hash.insert(Qt::UserRole + 4, QByteArray("weatherDescription"));
    m_hash.insert(Qt::UserRole + 5, QByteArray("temperatureMin"));
    m_hash.insert(Qt::UserRole + 6, QByteArray("temperatureMax"));
    m_hash.insert(Qt::UserRole + 7, QByteArray("windSpeed"));
    m_hash.insert(Qt::UserRole + 8, QByteArray("windGusts"));
    m_hash.insert(Qt::UserRole + 9, QByteArray("windDirection"));

    m_db = dbAdapter::instance().getDatabase();
    m_api = new OpenWeatherAPI();

    connect(this, &WeatherModel::cityChanged, this, &WeatherModel::loadWeatherFromDB);
    connect(this, &WeatherModel::cityChanged, this, &WeatherModel::loadWeatherFromAPI);
    connect(m_api, &OpenWeatherAPI::weatherChanged, this, &WeatherModel::loadWeatherFromDB);
    connect(m_api, &OpenWeatherAPI::cityNotFound, this, &WeatherModel::cityNotFound);
}

int WeatherModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_weatcherList.count();
}

QVariant WeatherModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_weatcherList.size())
        return QVariant();

    WeatherData* item = m_weatcherList.at(index.row());
    switch (role) {
    case Qt::UserRole:
        return item->cityName();
    case Qt::UserRole + 1:
        return item->cityID();
    case Qt::UserRole + 2:
        return item->dateTime();
    case Qt::UserRole + 3:
        return item->weatherIcon();
    case Qt::UserRole + 4:
        return item->weatherDescription();
    case Qt::UserRole + 5:
        return item->temperatureMin();
    case Qt::UserRole + 6:
        return item->temperatureMax();
    case Qt::UserRole + 7:
        return item->windSpeed();
    case Qt::UserRole + 8:
        return item->windGusts();
    case Qt::UserRole + 9:
        return item->windDirection();
    default:
        return QVariant();
    }
}

void WeatherModel::setCity(QString city)
{
    if (city != m_city) {
        m_city = city;
        emit cityChanged();
    }
}

QVariantMap WeatherModel::get(int row) const
{
    QVariantMap res;
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QModelIndex idx = index(row, 0);
    while (i.hasNext()) {
        i.next();
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }

    return res;
}

bool WeatherModel::hasValidWeather()
{
    return m_weatcherList.count() == 5;
}

QString WeatherModel::niceTemperatureString(int temperatureUnits, double t)
{
    double fh;
    switch (temperatureUnits) {
    case 2: // farnheit
        fh = (t - ZERO_KELVIN) * 1.8 + 32;
        return QString::number(qRound(fh));
    case 1: // celsius
        return QString::number(qRound(t - ZERO_KELVIN));
    case 0: // kelvin
    default:
        return QString::number(qRound(t));
        break;
    }
}

double WeatherModel::speedConvert(int units, double s)
{
    switch (units) {
    case 0: // Mph
        return s * 2.23693629;
    case 2: // km/h
        return s * 3.6;
        ;
    case 3: // kt
        return s * 1.94384449;
    default:
    case 1: // m/s
        return s;
    }
}

void WeatherModel::loadWeatherFromDB()
{
    if (m_city.isEmpty()) {
        qWarning() << "City is empty";
    }

    beginResetModel();
    m_weatcherList.clear();
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM weather WHERE cityName=:City ORDER BY datetime DESC");
    query.bindValue(":City", m_city);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery() << query.lastError().text();
    }

    while (query.next()) {
        WeatherData* weatcher = new WeatherData(query.value(0).toInt());
        if (weatcher->isValid()) {
            m_weatcherList.push_back(weatcher);
        }
    }

    endResetModel();
    emit hasValidWeatherChanged();
}

void WeatherModel::loadWeatherFromAPI()
{
    m_api->refreshWeather(m_city);
}
