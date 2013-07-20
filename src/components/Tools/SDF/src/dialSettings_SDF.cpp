#include <mepp_config.h>
#ifdef BUILD_component_SDF

#include "dialSettings_SDF.hxx"

SettingsDialog_SDF::SettingsDialog_SDF(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    loadDefaults();
    loadFromSettings();
}

void SettingsDialog_SDF::loadDefaults()
{
}

void SettingsDialog_SDF::loadFromSettings()
{
}

void SettingsDialog_SDF::saveToSettings()
{
}

void SettingsDialog_SDF::accept()
{
    saveToSettings();

    QDialog::accept();
}

#endif
