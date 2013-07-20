#include <mepp_config.h>
#ifdef BUILD_component_HKS

#include "dialSettings_HKS.hxx"

SettingsDialog_HKS::SettingsDialog_HKS(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    loadDefaults();
    loadFromSettings();
}

void SettingsDialog_HKS::loadDefaults()
{
}

void SettingsDialog_HKS::loadFromSettings()
{
}

void SettingsDialog_HKS::saveToSettings()
{
}

void SettingsDialog_HKS::accept()
{
    saveToSettings();

    QDialog::accept();
}

#endif
