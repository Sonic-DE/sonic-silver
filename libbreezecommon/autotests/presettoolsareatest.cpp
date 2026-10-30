/*
 * SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

// Silver presets and the shared titlebar transparency (KToolsArea): applying a preset
// sets it, Silver keeps none of it, and saving a preset records it.

#include "breezesettings.h"
#include "presetsmodel.h"

#include <KConfigGroup>
#include <KToolsArea>

#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

using namespace Breeze;

class PresetToolsAreaTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testApplyingAPresetSetsSharedSettings();
    void testSavingAPresetRecordsSharedSettings();

private:
    QTemporaryDir m_dir;
};

void PresetToolsAreaTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void PresetToolsAreaTest::testApplyingAPresetSetsSharedSettings()
{
    KConfig presets(m_dir.filePath(QStringLiteral("windecopresetsrc")), KConfig::SimpleConfig);
    KConfigGroup preset = presets.group(QStringLiteral("Windeco Preset Glass"));
    preset.writeEntry("ActiveTitleBarOpacity", 40);
    preset.writeEntry("BlurTransparentTitleBars", false);
    presets.sync();
    KConfig silverrc(m_dir.filePath(QStringLiteral("silverrc")), KConfig::SimpleConfig);

    InternalSettings settings;
    QVERIFY(PresetsModel::loadPresetAndSave(&settings, &silverrc, &presets, QStringLiteral("Glass"), false));

    const KSharedConfigPtr kdeglobals = KSharedConfig::openConfig(QStringLiteral("kdeglobals"));
    kdeglobals->reparseConfiguration();
    const KToolsArea::Settings shared = KToolsArea::settings(kdeglobals);
    QVERIFY(shared.enabled);
    QCOMPARE(shared.activeOpacity, 40);
    QCOMPARE(shared.inactiveOpacity, 100);
    QVERIFY(!shared.blurBehind);
    // Silver doesn't keep them.
    QVERIFY(!silverrc.group(QStringLiteral("TitleBarOpacity")).hasKey("ActiveTitleBarOpacity"));
    QVERIFY(!silverrc.group(QStringLiteral("TitleBarOpacity")).hasKey("BlurTransparentTitleBars"));
}

void PresetToolsAreaTest::testSavingAPresetRecordsSharedSettings()
{
    const KSharedConfigPtr kdeglobals = KSharedConfig::openConfig(QStringLiteral("kdeglobals"));
    KToolsArea::Settings shared;
    shared.activeOpacity = 70;
    shared.matchApplicationColor = true;
    KToolsArea::writeSettings(kdeglobals, shared);
    kdeglobals->sync();

    KConfig presets(m_dir.filePath(QStringLiteral("saved-presetsrc")), KConfig::SimpleConfig);
    InternalSettings settings;
    PresetsModel::writePreset(&settings, &presets, QStringLiteral("Mine"));
    const KConfigGroup preset = presets.group(QStringLiteral("Windeco Preset Mine"));
    QCOMPARE(preset.readEntry("ActiveTitleBarOpacity", 0), 70);
    QCOMPARE(preset.readEntry("MatchTitleBarToApplicationColor", false), true);
}

QTEST_GUILESS_MAIN(PresetToolsAreaTest)

#include "presettoolsareatest.moc"
