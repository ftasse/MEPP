#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#include "dialSettings_Skeleton_Extraction.hxx"

SettingsDialog_Skeleton_Extraction::SettingsDialog_Skeleton_Extraction(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    loadDefaults();
    loadFromSettings();
}

void SettingsDialog_Skeleton_Extraction::loadDefaults()
{
}

void SettingsDialog_Skeleton_Extraction::loadFromSettings()
{
}

void SettingsDialog_Skeleton_Extraction::saveToSettings()
{
}

void SettingsDialog_Skeleton_Extraction::accept()
{
    saveToSettings();

    QDialog::accept();
}

#endif
