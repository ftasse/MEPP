///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#define BUILD_component_SDF

#include <mepp_config.h>
#ifdef BUILD_component_SDF

#include "mepp_component_SDF_plugin.hxx"

#include <QObject>
#include <QAction>
#include <QApplication>
#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "dialSettings_SDF.hxx"
#include "SDF_Component.h"
typedef boost::shared_ptr<SDF_Component> SDF_ComponentPtr;
typedef boost::shared_ptr<SettingsDialog_SDF> SettingsDialogPtr;

SettingsDialogPtr create_dialog_settings(SDF_ComponentPtr component_ptr)
{
    SettingsDialogPtr dialog_ptr = SettingsDialogPtr(new SettingsDialog_SDF);
    dialog_ptr->coneAngInDegs->setValue(component_ptr->coneAngInDegs);
    dialog_ptr->numConeRays->setValue(component_ptr->numConeRays);
    dialog_ptr->numSmoothingIter->setValue(component_ptr->numSmoothingIters);
    dialog_ptr->perFacet->setChecked(component_ptr->sdfPerFacet);
    return dialog_ptr;
}

void get_dialog_settings(SettingsDialogPtr dialog_ptr, SDF_ComponentPtr component_ptr)
{
    component_ptr->coneAngInDegs = dialog_ptr->coneAngInDegs->value();
    component_ptr->numConeRays = dialog_ptr->numConeRays->value();
    component_ptr->numSmoothingIters = dialog_ptr->numSmoothingIter->value();
    component_ptr->sdfPerFacet = dialog_ptr->perFacet->isChecked();
    dialog_ptr.reset();
}

void mepp_component_SDF_plugin::step1()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        SDF_ComponentPtr component_ptr = findOrCreateComponentForViewer<SDF_ComponentPtr, SDF_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->computeSDF();

            mw->statusBar()->showMessage(tr("Compute SDF...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Compute SDF ... canceled"));
    }
}

void mepp_component_SDF_plugin::step2()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        SDF_ComponentPtr component_ptr = findOrCreateComponentForViewer<SDF_ComponentPtr, SDF_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getOpenFileName(viewer, "Load a SDF file",
                                                     QString(), tr("SDF Files (*.sdf)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->loadSDF(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("load SDF...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("load SDF...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("load skeleton...canceled"));
        }
    }
}

void mepp_component_SDF_plugin::step3()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        SDF_ComponentPtr component_ptr = findOrCreateComponentForViewer<SDF_ComponentPtr, SDF_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getSaveFileName(viewer, "Save SDF",
                                                     QString(), tr("SDF Files (*.sdf)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->saveSDF(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("save SDF...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("save SDF...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("save SDF...canceled"));
        }
    }
}

void mepp_component_SDF_plugin::step4()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        SDF_ComponentPtr component_ptr = findOrCreateComponentForViewer<SDF_ComponentPtr, SDF_Component>(viewer, polyhedron_ptr);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        component_ptr->set_init(8);
        component_ptr->sdfToColorMap();
        viewer->recreateListsAndUpdateGL();

        mw->statusBar()->showMessage(tr("sdf to color map...done"));
        QApplication::restoreOverrideCursor();
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mepp_component_SDF_plugin, mepp_component_SDF_plugin);
#endif

#endif
