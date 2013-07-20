#ifndef HEADER_MEPP_COMPONENT_Partition_PLUGIN_SETTINGS_H
#define HEADER_MEPP_COMPONENT_Partition_PLUGIN_SETTINGS_H

#include <mepp_config.h>
#ifdef BUILD_component_Partition

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "ui_dialSettings_Partition.h"

class SettingsDialog_Partition : public QDialog, public Ui_Settings
{
    Q_OBJECT

public:
    SettingsDialog_Partition(QWidget *parent = 0);
    void accept();

private slots:
    void loadDefaults();
    void loadFromSettings();
    void saveToSettings();

private:
};

#endif

#endif // HEADER_MEPP_COMPONENT_Partition_PLUGIN_SETTINGS_H
