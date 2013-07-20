#include <mepp_config.h>
#ifdef BUILD_component_Partition

#include "dialSettings_Partition.hxx"

SettingsDialog_Partition::SettingsDialog_Partition(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    loadDefaults();
    loadFromSettings();
}

void SettingsDialog_Partition::loadDefaults()
{
}

void SettingsDialog_Partition::loadFromSettings()
{
}

void SettingsDialog_Partition::saveToSettings()
{
}

void SettingsDialog_Partition::accept()
{
    saveToSettings();

    QDialog::accept();
}

#endif
