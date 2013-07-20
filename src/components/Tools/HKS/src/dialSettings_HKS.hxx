#ifndef HEADER_MEPP_COMPONENT_HKS_PLUGIN_SETTINGS_H
#define HEADER_MEPP_COMPONENT_HKS_PLUGIN_SETTINGS_H

#include <mepp_config.h>
#ifdef BUILD_component_HKS

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "ui_dialSettings_HKS.h"

class SettingsDialog_HKS : public QDialog, public Ui_Settings
{
    Q_OBJECT

public:
    SettingsDialog_HKS(QWidget *parent = 0);
    void accept();

private slots:
    void loadDefaults();
    void loadFromSettings();
    void saveToSettings();

private:
};

#endif

#endif // HEADER_MEPP_COMPONENT_HKS_PLUGIN_SETTINGS_H
