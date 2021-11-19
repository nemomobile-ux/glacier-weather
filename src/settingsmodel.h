#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>

class SettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit SettingsModel(QObject *parent = nullptr);

signals:

};

#endif // SETTINGSMODEL_H
