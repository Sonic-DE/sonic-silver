//////////////////////////////////////////////////////////////////////////////
// main.cpp
// silver-settings executable
// -------------------
//
// SPDX-FileCopyrightText: 2010 Hugo Pereira Da Costa <hugo.pereira@free.fr>
// SPDX-FileCopyrightText: 2021-2024 Paul A McAuley <kde@paulmcauley.com>
// SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
//
// SPDX-License-Identifier: MIT
//////////////////////////////////////////////////////////////////////////////

#include "breeze.h"
#include "dbusmessages.h"
#include "presetsmodel.h"
#include <QAbstractScrollArea>
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>

#include <KCMultiDialog>
#include <KConfig>
#include <KLocalizedString>
#include <KPluginMetaData>

using namespace Breeze;
struct CommandLineProcessResult {
    enum class Status {
        CommandsProcessedOk,
        Error,
        NoCommand
    };
    Status statusCode = Status::NoCommand;
};
CommandLineProcessResult processComandLine(QApplication &app, QCommandLineParser &parser);

//__________________________________________
int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain("silver_style_config");
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("silver-settings"));
    app.setApplicationVersion(Breeze::silverLongVersion());

    QCommandLineParser parser;
    CommandLineProcessResult commandLineResult = processComandLine(app, parser);
    if (commandLineResult.statusCode == CommandLineProcessResult::Status::Error) {
        return 1;
    }
    if (commandLineResult.statusCode == CommandLineProcessResult::Status::CommandsProcessedOk) {
        return 0;
    }

    KCMultiDialog dialog;
    dialog.setWindowTitle(i18n("Silver Settings"));
    dialog.setMinimumWidth(800);
    dialog.addModule(KPluginMetaData(QStringLiteral("kstyle_config/silverstyleconfig")));
    dialog.addModule(KPluginMetaData(QStringLiteral("org.kde.kdecoration3.kcm/kcm_silverdecoration.so")));
    dialog.show();

    const auto children = dialog.findChildren<QAbstractScrollArea *>();
    for (auto child : children) {
        child->adjustSize();
        child->viewport()->adjustSize();
    }

    return app.exec();
}

CommandLineProcessResult processComandLine(QApplication &app, QCommandLineParser &parser)
{
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption importOption(QStringList() << QStringLiteral("i") << QStringLiteral("import-preset"),
                                    i18n("Import a Silver .klpw Preset File with filename <preset filename>."),
                                    i18n("preset filename"));
    parser.addOption(importOption);

    QCommandLineOption loadWindecoPresetOption(QStringList() << QStringLiteral("w") << QStringLiteral("load-windeco-preset"),
                                               i18n("Load the Window Decoration Preset, with name <preset name>."),
                                               i18n("preset name"));
    parser.addOption(loadWindecoPresetOption);

    QCommandLineOption forceOption(QStringList() << QStringLiteral("f") << QStringLiteral("force-import-invalid-version"),
                                   i18n("Force the import of a preset file from a different Silver version."));
    parser.addOption(forceOption);

    parser.process(app);

    QString const configFile = QStringLiteral("silver/silverrc");
    QString const presetsConfigFile = QStringLiteral("silver/windecopresetsrc");
    QTextStream output(stdout);
    bool commandSet = false;
    if (parser.isSet(importOption)) {
        commandSet = true;
        KSharedConfig::Ptr presetsConfig(KSharedConfig::openConfig(presetsConfigFile));
        QString presetName;
        QString errorMessage;
        PresetsErrorFlag importErrors =
            PresetsModel::importPreset(presetsConfig.data(), parser.value(importOption), presetName, errorMessage, parser.isSet(forceOption));
        if (importErrors == PresetsErrorFlag::InvalidGlobalGroup) {
            output << i18n("ERROR: Invalid Silver Preset file to import at \"") << parser.value(importOption) << i18n("\".") << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }

        if (importErrors == PresetsErrorFlag::InvalidVersion) {
            output << i18n("ERROR: The file to import at \"") << parser.value(importOption)
                   << i18n("\" was created for a different version of Silver.\n To force import, use the --force-import-invalid-version option.") << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }

        if (importErrors == PresetsErrorFlag::InvalidGroup) {
            output << i18n("ERROR: No preset group found in Silver Preset file at \"") << parser.value(importOption) << i18n("\".") << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }

        if (importErrors == PresetsErrorFlag::InvalidKey) {
            output << i18n("ERROR: Invalid key \"") << errorMessage << i18n("\" in Silver Preset file at \"") << parser.value(importOption) << i18n("\".")
                   << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }

        output << i18n("Preset, \"") << presetName << i18n("\" imported.") << Qt::endl;
    }

    if (parser.isSet(loadWindecoPresetOption)) {
        commandSet = true;
        KSharedConfig::Ptr config(KSharedConfig::openConfig(configFile));
        KSharedConfig::Ptr presetsConfig(KSharedConfig::openConfig(presetsConfigFile));

        PresetsModel::importBundledPresets(presetsConfig.data());

        if (!PresetsModel::isPresetPresent(presetsConfig.data(), parser.value(loadWindecoPresetOption))) {
            output << i18n("ERROR: Preset, \"") << parser.value(loadWindecoPresetOption) << i18n("\" not found.") << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }

        InternalSettingsPtr internalSettings = InternalSettingsPtr(new InternalSettings());
        internalSettings->load();
        if (!PresetsModel::loadPresetAndSave(internalSettings.data(), config.data(), presetsConfig.data(), parser.value(loadWindecoPresetOption), true)) {
            output << i18n("ERROR: Preset, \"") << parser.value(loadWindecoPresetOption) << i18n("\" could not be loaded.") << Qt::endl;
            return {CommandLineProcessResult::Status::Error};
        }
        DBusMessages::updateDecorationColorCache();
        // needed to tell kwin to reload when running from external silver-settings
        DBusMessages::kwinReloadConfig();
        DBusMessages::kstyleReloadDecorationConfig();
        DBusMessages::kstyleReloadConfig();

        output << i18n("Preset, \"") << parser.value(loadWindecoPresetOption) << i18n("\" loaded...") << Qt::endl;
    }

    if (commandSet) {
        return {CommandLineProcessResult::Status::CommandsProcessedOk};
    } else {
        return {CommandLineProcessResult::Status::NoCommand};
    }
}
