///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#define BUILD_component_HKS

#include <mepp_config.h>
#ifdef BUILD_component_HKS

#include "mepp_component_HKS_plugin.hxx"

#include <QObject>
#include <QAction>
#include <QApplication>
#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "dialSettings_HKS.hxx"
#include "HKS_Component.h"
typedef boost::shared_ptr<HKS_Component> HKS_ComponentPtr;
typedef boost::shared_ptr<SettingsDialog_HKS> SettingsDialogPtr;

SettingsDialogPtr create_dialog_settings(HKS_ComponentPtr component_ptr)
{
    SettingsDialogPtr dialog_ptr = SettingsDialogPtr(new SettingsDialog_HKS);
    dialog_ptr->nev->setValue(component_ptr->nev);
    dialog_ptr->nsteps->setValue(component_ptr->nsteps);
    dialog_ptr->useGeodesic->setChecked(component_ptr->useGeodesic);
    return dialog_ptr;
}

void get_dialog_settings(SettingsDialogPtr dialog_ptr, HKS_ComponentPtr component_ptr)
{
    component_ptr->nev = dialog_ptr->nev->value();
    component_ptr->nsteps = dialog_ptr->nsteps->value();
    component_ptr->useGeodesic = dialog_ptr->useGeodesic->isChecked();
    dialog_ptr.reset();
}

void mepp_component_HKS_plugin::OnKeyPress(QKeyEvent *event)
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();

        if (doesExistComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr)) // important !!!
        {
            HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

            if (event->key() == Qt::Key_E || event->key() == Qt::Key_D)
            {
                int nev = polyhedron_ptr->evals.size();
                if (nev > 0)
                {
                    if (event->key() == Qt::Key_E)
                        component_ptr->eig_id = (component_ptr->eig_id+1)%nev;
                    else
                        component_ptr->eig_id = component_ptr->eig_id>0?component_ptr->eig_id-1:nev-1;
                    step5();
                    std::stringstream ss;
                    ss << "Showing  eigenvector at position e=" << component_ptr->eig_id+1
                       << "/" << nev;
                    mw->statusBar()->showMessage(tr(ss.str().c_str()), 1000);
                }
            } else if (event->key() == Qt::Key_H || event->key() == Qt::Key_N)
            {
                int nsteps = (polyhedron_ptr->vertices_begin())->hks.size();
                if (nsteps > 0)
                {
                    if (event->key() == Qt::Key_H)
                        component_ptr->hks_tid = (component_ptr->hks_tid+1)%nsteps;
                    else
                        component_ptr->hks_tid = component_ptr->hks_tid>0?component_ptr->hks_tid-1:nsteps-1;
                    step6();
                    std::stringstream ss;
                    ss << "Showing  hks at time position t=" << component_ptr->hks_tid+1
                          << "/" << nsteps;
                    mw->statusBar()->showMessage(tr(ss.str().c_str()), 1000);
                }
            }
        }
    }
}


void mepp_component_HKS_plugin::step1()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->computeSpectra();
            component_ptr->set_init(2);

            mw->statusBar()->showMessage(tr("Contract mesh...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Contract mesh ... canceled"));
    }
}

void mepp_component_HKS_plugin::step2()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->computeHKS();
            component_ptr->set_init(3);

            mw->statusBar()->showMessage(tr("Simplify contracted mesh...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Simplify contracted mesh...canceled"));
    }
}

void mepp_component_HKS_plugin::step3()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getOpenFileName(viewer, "Load a HKS file",
                                                     QString(), tr("HKS Files (*.skel)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->loadHKS(fname.toStdString()))
            {
                component_ptr->set_init(6);
                mw->statusBar()->showMessage(tr("load HKS...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("load HKS...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("load HKS...canceled"));
        }
    }
}

void mepp_component_HKS_plugin::step4()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getSaveFileName(viewer, "Save HKS",
                                                     QString(), tr("HKS Files (*.skel)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->saveHKS(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("save HKS...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("save HKS...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("save HKS...canceled"));
        }
    }
}

void mepp_component_HKS_plugin::step5()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        component_ptr->spectraToColorMap();
        viewer->recreateListsAndUpdateGL();

        mw->statusBar()->showMessage(tr("spectra to vertex color map...done"));
        QApplication::restoreOverrideCursor();
    }
}

void mepp_component_HKS_plugin::step6()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        HKS_ComponentPtr component_ptr = findOrCreateComponentForViewer<HKS_ComponentPtr, HKS_Component>(viewer, polyhedron_ptr);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        component_ptr->hksToColorMap();
        viewer->recreateListsAndUpdateGL();

        mw->statusBar()->showMessage(tr("hks to vertex color map...done"));
        QApplication::restoreOverrideCursor();
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mepp_component_HKS_plugin, mepp_component_HKS_plugin);
#endif

#endif
