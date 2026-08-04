/*
 * SPDX-FileCopyrightText: 2024 Paul A McAuley <kde@paulmcauley.com>
 * SPDX-FileCopyrightText: 2026 Joseph Crowell <joseph.w.crowell@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "systemicongeneration.h"
#include "breezeconfigwidget.h"
#include "presetsmodel.h"
#include <QPushButton>
#include <kicontheme.h>

namespace Breeze
{

class ConfigWidget;

SystemIconGeneration::SystemIconGeneration(KSharedConfig::Ptr config, KSharedConfig::Ptr presetsConfig, QObject *parent)
    : QDialog(static_cast<ConfigWidget *>(parent)->widget())
    , m_ui(new Ui_SystemIconGeneration)
    , m_configuration(config)
    , m_presetsConfiguration(presetsConfig)
    , m_parent(parent)
{
    m_ui->setupUi(this);
    this->setWindowTitle(i18n("System Icon Generation - Silver Settings"));
    populateComboBoxesWithSystemIconThemes();

    connect(m_ui->silverIconThemeInherits, &QComboBox::currentTextChanged, this, &SystemIconGeneration::updateChanged, Qt::ConnectionType::DirectConnection);
    connect(m_ui->silverDarkIconThemeInherits,
            &QComboBox::currentTextChanged,
            this,
            &SystemIconGeneration::updateChanged,
            Qt::ConnectionType::DirectConnection);

    connect(m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QAbstractButton::clicked, this, &SystemIconGeneration::defaults);
    connect(m_ui->buttonBox->button(QDialogButtonBox::Reset), &QAbstractButton::clicked, this, &SystemIconGeneration::load);
    connect(m_ui->buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked, this, &SystemIconGeneration::saveAndReloadKWinConfig);
    setApplyButtonState(false);
}

SystemIconGeneration::~SystemIconGeneration()
{
    delete m_ui;
}

void SystemIconGeneration::load()
{
    m_loading = true;

    // create internal settings and load from rc files
    m_internalSettings = InternalSettingsPtr(new InternalSettings());
    m_internalSettings->load();

    m_ui->silverIconThemeInherits->setCurrentText(m_internalSettings->silverIconThemeInherits());
    m_ui->silverDarkIconThemeInherits->setCurrentText(m_internalSettings->silverDarkIconThemeInherits());

    setChanged(false);

    m_loading = false;
    m_loaded = true;
}

void SystemIconGeneration::populateComboBoxesWithSystemIconThemes()
{
    QStringList iconThemes = KIconTheme::list();
    iconThemes.removeAll(QStringLiteral("silver-generated"));
    iconThemes.removeAll(QStringLiteral("silver-dark-generated"));
    iconThemes.sort(Qt::CaseSensitivity::CaseInsensitive);

    m_ui->silverIconThemeInherits->addItems(iconThemes);
    m_ui->silverDarkIconThemeInherits->addItems(iconThemes);
}

void SystemIconGeneration::save(const bool reloadKwinConfig)
{
    // create internal settings and load from rc files
    m_internalSettings = InternalSettingsPtr(new InternalSettings());
    m_internalSettings->load();

    m_internalSettings->setSilverIconThemeInherits(m_ui->silverIconThemeInherits->currentText());
    m_internalSettings->setSilverDarkIconThemeInherits(m_ui->silverDarkIconThemeInherits->currentText());

    m_internalSettings->save();
    setChanged(false);

    if (reloadKwinConfig) {
        static_cast<ConfigWidget *>(m_parent)->generateSystemIcons();
    }
}

void SystemIconGeneration::defaults()
{
    m_processingDefaults = true;
    // create internal settings and load from rc files
    m_internalSettings = InternalSettingsPtr(new InternalSettings());
    m_internalSettings->setDefaults();

    populateComboBoxesWithSystemIconThemes();
    m_ui->silverIconThemeInherits->setCurrentText(m_internalSettings->silverIconThemeInherits());
    m_ui->silverDarkIconThemeInherits->setCurrentText(m_internalSettings->silverDarkIconThemeInherits());

    setChanged(!isDefaults());

    m_processingDefaults = false;
    m_defaultsPressed = true;
}

bool SystemIconGeneration::isDefaults()
{
    bool isDefaults = true;

    QString groupName(QStringLiteral("SystemIconGeneration"));
    if (m_configuration->hasGroup(groupName)) {
        KConfigGroup group = m_configuration->group(groupName);
        if (group.keyList().count())
            isDefaults = false;
    }

    return isDefaults;
}

void SystemIconGeneration::setChanged(bool value)
{
    m_changed = value;
    setApplyButtonState(value);
    Q_EMIT changed(value);
}

void SystemIconGeneration::accept()
{
    save();
    QDialog::accept();
}

void SystemIconGeneration::reject()
{
    load();
    QDialog::reject();
}

void SystemIconGeneration::updateChanged()
{
    // check configuration
    if (!m_internalSettings)
        return;

    if (m_loading)
        return; // only check if the user has made a change to the UI, or user has pressed defaults

    // track modifications
    bool modified(false);

    if (m_ui->silverIconThemeInherits->currentText() != m_internalSettings->silverIconThemeInherits())
        modified = true;
    else if (m_ui->silverDarkIconThemeInherits->currentText() != m_internalSettings->silverDarkIconThemeInherits())
        modified = true;

    setChanged(modified);
}

void SystemIconGeneration::setApplyButtonState(const bool on)
{
    m_ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(on);
}

}
