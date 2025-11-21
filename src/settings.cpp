/****************************************************************************************
**
** Copyright (C) 2025 Chupligin Sergey <neochapay@gmail.com>
** All rights reserved.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the author nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
** ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

#include "settings.h"

Settings::Settings(QObject* parent)
    : QSettings(parent)
{
}

Settings::~Settings()
{
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue = QVariant())
{
    return QSettings::value(key, defaultValue);
}

bool Settings::boolValue(const QString& key, bool defaultValue)
{
    return QSettings::value(key, defaultValue).toBool();
}

void Settings::setValue(const QString& key, const QVariant& value)
{

    // change the setting and emit a changed signal
    // (we are not checking if the value really changed before emitting for simplicity)
    QSettings::setValue(key, value);
    emit settingChanged(key);
}

void Settings::setValueIfNotSet(const QString& key, const QVariant& value)
{

    // change the setting and emit a changed signal
    if (!QSettings::contains(key)) {
        QSettings::setValue(key, value);
        // (we are not checking if the value really changed before emitting for simplicity)
        emit settingChanged(key);
    }
}

void Settings::initToDefaults()
{
    setValueIfNotSet("test", true);
}
