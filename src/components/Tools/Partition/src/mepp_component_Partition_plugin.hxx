///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef HEADER_MEPP_COMPONENT_Partition_PLUGIN_INTERFACE_H
#define HEADER_MEPP_COMPONENT_Partition_PLUGIN_INTERFACE_H

#include <QtGlobal> // important, for QT_VERSION

#include <QObject>

#include <mepp_config.h>
#ifdef BUILD_component_Partition

#include "../../../../mepp/mepp_component_plugin_interface.h"

#include <QAction>
#include <QtPlugin>

class mepp_component_Partition_plugin :
        public QObject,
        public mepp_component_plugin_interface
{
    Q_OBJECT
    Q_INTERFACES(mepp_component_plugin_interface);
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "mepp_component_Partition_plugin")
#endif

public:
    mepp_component_Partition_plugin() : mepp_component_plugin_interface() {}
    ~mepp_component_Partition_plugin()
    {
        delete actionStep_1; delete actionStep_2; delete actionStep_3;
        delete actionStep_4; delete actionStep_5;
    }

    void init(mainwindow* mainWindow, QList<QMdiSubWindow *> lw)
    {
        this->mw = mainWindow;
        this->lwindow = lw;
        this->mPluginName = this->metaObject()->className();

        mParentMenu = mainWindow->menuExamples;

        actionStep_1 = new QAction(tr("Segment mesh"), this);
        if (actionStep_1)
            connect(actionStep_1, SIGNAL(triggered()), this, SLOT(step1()));

        actionStep_2 = new QAction(tr("Load labelling"), this);
        if (actionStep_2)
            connect(actionStep_2, SIGNAL(triggered()), this, SLOT(step2()));

        actionStep_3 = new QAction(tr("Save labelling"), this);
        if (actionStep_3)
            connect(actionStep_3, SIGNAL(triggered()), this, SLOT(step3()));

        actionStep_4 = new QAction(tr("Save partitions as meshes"), this);
        if (actionStep_4)
            connect(actionStep_4, SIGNAL(triggered()), this, SLOT(step4()));

        actionStep_5 = new QAction(tr("Label To Face Color Map"), this);
        if (actionStep_5)
            connect(actionStep_5, SIGNAL(triggered()), this, SLOT(step5()));

    }

    QList<QAction*> actions() const
    {
        return QList<QAction*>()	//<< actionExample
                << actionStep_1
                << NULL
                << actionStep_2
                << actionStep_3
                << actionStep_4
                << NULL
                << actionStep_5;
    }

private:

public slots:
    void step1();
    void step2();
    void step3();
    void step4();
    void step5();

private:
    QAction *actionStep_1, *actionStep_2, *actionStep_3, *actionStep_4;
    QAction *actionStep_5;
};

#endif

#endif
