/*
 * SPDX-FileCopyrightText: 2024 Paul A McAuley <kde@paulmcauley.com>
 * SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

#include "breeze.h"
#include "breezecommon_export.h"
#include <QDBusVariant>
#include <QString>

namespace Breeze
{

class BREEZECOMMON_EXPORT DBusUpdateNotifier : public QObject
{
    Q_OBJECT

public:
    DBusUpdateNotifier();

public Q_SLOTS:
    void onWindowDecorationSettingsUpdate();
    void onSystemSettingUpdate(QString, QString, QDBusVariant);
    // KGlobalSettings.notifyChange: a [ToolsArea] change (the theme-independent titlebar
    // transparency settings) is a decoration settings update.
    void onGlobalSettingsChange();

Q_SIGNALS:
    void decorationSettingsUpdate(QByteArray uuid);
    void systemColorSchemeUpdate(QByteArray uuid);
    void systemIconsUpdate();

private:
    QString m_toolsAreaSnapshot;
};

extern DBusUpdateNotifier BREEZECOMMON_EXPORT g_dBusUpdateNotifier;

}
