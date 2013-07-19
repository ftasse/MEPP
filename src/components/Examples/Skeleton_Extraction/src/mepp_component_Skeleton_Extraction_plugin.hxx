///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef HEADER_MEPP_COMPONENT_Skeleton_Extraction_PLUGIN_INTERFACE_H
#define HEADER_MEPP_COMPONENT_Skeleton_Extraction_PLUGIN_INTERFACE_H

#include <QtGlobal> // important, for QT_VERSION

#include <QObject>

#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#include "../../../../mepp/mepp_component_plugin_interface.h"

#include <QAction>
#include <QtPlugin>

class mepp_component_Skeleton_Extraction_plugin :
        public QObject,
        public mepp_component_plugin_interface
{
    Q_OBJECT
    Q_INTERFACES(mepp_component_plugin_interface);
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "mepp_component_Skeleton_Extraction_plugin")
#endif

public:
    mepp_component_Skeleton_Extraction_plugin() : mepp_component_plugin_interface() {}
    ~mepp_component_Skeleton_Extraction_plugin()
    {
        delete actionStep_1; delete actionStep_2; delete actionStep_3;
        delete actionStep_4; delete actionStep_5; delete actionStep_6;
        delete actionStep_7;
    }

    void init(mainwindow* mainWindow, QList<QMdiSubWindow *> lw)
    {
        this->mw = mainWindow;
        this->lwindow = lw;
        this->mPluginName = this->metaObject()->className();

        mParentMenu = mainWindow->menuExamples;

        actionStep_1 = new QAction(tr("Contract"), this);
        if (actionStep_1)
            connect(actionStep_1, SIGNAL(triggered()), this, SLOT(step1()));

        actionStep_2 = new QAction(tr("Simplify"), this);
        if (actionStep_2)
            connect(actionStep_2, SIGNAL(triggered()), this, SLOT(step2()));

        actionStep_3 = new QAction(tr("Refine Embedding"), this);
        if (actionStep_3)
            connect(actionStep_3, SIGNAL(triggered()), this, SLOT(step3()));

        actionStep_4 = new QAction(tr("Extract Skeleton"), this);
        if (actionStep_4)
            connect(actionStep_4, SIGNAL(triggered()), this, SLOT(step4()));

        actionStep_5 = new QAction(tr("Load Skeleton"), this);
        if (actionStep_5)
            connect(actionStep_5, SIGNAL(triggered()), this, SLOT(step5()));

        actionStep_6 = new QAction(tr("Save Skeleton"), this);
        if (actionStep_6)
            connect(actionStep_6, SIGNAL(triggered()), this, SLOT(step6()));

        actionStep_7 = new QAction(tr("Thickness To Vertex Color Map"), this);
        if (actionStep_7)
            connect(actionStep_7, SIGNAL(triggered()), this, SLOT(step7()));

    }

    QList<QAction*> actions() const
    {
        return QList<QAction*>()	//<< actionExample
                << actionStep_1
                << actionStep_2
                << actionStep_3
                << NULL			// menu separator
                << actionStep_4
                << NULL
                << actionStep_5
                << actionStep_6
                << NULL
                << actionStep_7;
    }

    virtual void pre_draw();

private:

    void draw_skeleton(Viewer* viewer);
    void draw_contracted_mesh(Viewer* viewer);

public slots:
    void step1();
    void step2();
    void step3();
    void step4();
    void step5();
    void step6();
    void step7();

private:
    QAction *actionStep_1, *actionStep_2, *actionStep_3, *actionStep_4;
    QAction *actionStep_5, *actionStep_6, *actionStep_7;
};

#endif

#endif
