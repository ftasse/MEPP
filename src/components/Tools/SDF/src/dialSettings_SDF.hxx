#ifndef HEADER_MEPP_COMPONENT_SDF_PLUGIN_SETTINGS_H
#define HEADER_MEPP_COMPONENT_SDF_PLUGIN_SETTINGS_H

#include <mepp_config.h>
#ifdef BUILD_component_SDF

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "ui_dialSettings_SDF.h"

class SettingsDialog_SDF : public QDialog, public Ui_Settings
{
    Q_OBJECT

public:
    SettingsDialog_SDF(QWidget *parent = 0);
    void accept();

private slots:
    void loadDefaults();
    void loadFromSettings();
    void saveToSettings();

private:
};

#endif

#endif // HEADER_MEPP_COMPONENT_SDF_PLUGIN_SETTINGS_H
