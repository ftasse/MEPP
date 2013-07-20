///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef HEADER_MEPP_COMPONENT_SDF_PLUGIN_INTERFACE_H
#define HEADER_MEPP_COMPONENT_SDF_PLUGIN_INTERFACE_H

#include <QtGlobal> // important, for QT_VERSION

#include <QObject>

#include <mepp_config.h>
#ifdef BUILD_component_SDF

#include "../../../../mepp/mepp_component_plugin_interface.h"

#include <QAction>
#include <QtPlugin>

class mepp_component_SDF_plugin :
        public QObject,
        public mepp_component_plugin_interface
{
    Q_OBJECT
    Q_INTERFACES(mepp_component_plugin_interface);
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "mepp_component_SDF_plugin")
#endif

public:
    mepp_component_SDF_plugin() : mepp_component_plugin_interface() {}
    ~mepp_component_SDF_plugin()
    {
        delete actionStep_1; delete actionStep_2; delete actionStep_3;
        delete actionStep_4;
    }

    void init(mainwindow* mainWindow, QList<QMdiSubWindow *> lw)
    {
        this->mw = mainWindow;
        this->lwindow = lw;
        this->mPluginName = this->metaObject()->className();

        mParentMenu = mainWindow->menuExamples;

        actionStep_1 = new QAction(tr("Compute SDF"), this);
        if (actionStep_1)
            connect(actionStep_1, SIGNAL(triggered()), this, SLOT(step1()));

        actionStep_2 = new QAction(tr("Load SDF"), this);
        if (actionStep_2)
            connect(actionStep_2, SIGNAL(triggered()), this, SLOT(step2()));

        actionStep_3 = new QAction(tr("Save SDF"), this);
        if (actionStep_3)
            connect(actionStep_3, SIGNAL(triggered()), this, SLOT(step3()));

        actionStep_4 = new QAction(tr("SDF To Face Color Map"), this);
        if (actionStep_4)
            connect(actionStep_4, SIGNAL(triggered()), this, SLOT(step4()));

    }

    QList<QAction*> actions() const
    {
        return QList<QAction*>()	//<< actionExample
                << actionStep_1
                << actionStep_2
                << actionStep_3
                << NULL			// menu separator
                << actionStep_4;
    }

private:

public slots:
    void step1();
    void step2();
    void step3();
    void step4();

private:
    QAction *actionStep_1, *actionStep_2, *actionStep_3, *actionStep_4;
};

#endif

#endif
