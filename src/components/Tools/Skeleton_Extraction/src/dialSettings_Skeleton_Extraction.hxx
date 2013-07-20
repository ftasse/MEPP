#ifndef HEADER_MEPP_COMPONENT_Skeleton_Extraction_PLUGIN_SETTINGS_H
#define HEADER_MEPP_COMPONENT_Skeleton_Extraction_PLUGIN_SETTINGS_H

#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "ui_dialSettings_Skeleton_Extraction.h"

class SettingsDialog_Skeleton_Extraction : public QDialog, public Ui_Settings
{
    Q_OBJECT

public:
    SettingsDialog_Skeleton_Extraction(QWidget *parent = 0);
    void accept();

private slots:
    void loadDefaults();
    void loadFromSettings();
    void saveToSettings();

private:
};

#endif

#endif // HEADER_MEPP_COMPONENT_Skeleton_Extraction_PLUGIN_SETTINGS_H
