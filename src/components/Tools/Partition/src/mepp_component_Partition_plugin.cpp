///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#define BUILD_component_Partition

#include <mepp_config.h>
#ifdef BUILD_component_Partition

#include "mepp_component_Partition_plugin.hxx"

#include <QObject>
#include <QAction>
#include <QApplication>
#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "dialSettings_Partition.hxx"
#include "Partition_Component.h"
typedef boost::shared_ptr<Partition_Component> Partition_ComponentPtr;
typedef boost::shared_ptr<SettingsDialog_Partition> SettingsDialogPtr;

SettingsDialogPtr create_dialog_settings(Partition_ComponentPtr component_ptr)
{
    SettingsDialogPtr dialog_ptr = SettingsDialogPtr(new SettingsDialog_Partition);
    dialog_ptr->useMeshProp->setCurrentIndex(component_ptr->meshProperty);
    dialog_ptr->num_labels->setValue(component_ptr->numInitLabels);
    dialog_ptr->isRecursive->setChecked(component_ptr->isRecursive);
    dialog_ptr->isHierarchical->setChecked(component_ptr->isHierarchical);
    dialog_ptr->lambda->setValue(component_ptr->lambda);
    dialog_ptr->alpha->setValue(component_ptr->alpha);
    return dialog_ptr;
}

void get_dialog_settings(SettingsDialogPtr dialog_ptr, Partition_ComponentPtr component_ptr)
{
    switch (dialog_ptr->useMeshProp->currentIndex())
    {
    case Partition_Component::SDFProperty:
        component_ptr->meshProperty = Partition_Component::SDFProperty;
        break;
    case Partition_Component::HKSProperty:
        component_ptr->meshProperty = Partition_Component::HKSProperty;
        break;

    case Partition_Component::LPEigProperty:
        component_ptr->meshProperty = Partition_Component::LPEigProperty;
        break;
    }

    component_ptr->numInitLabels = dialog_ptr->num_labels->value();
    component_ptr->isRecursive = dialog_ptr->isRecursive->isChecked();
    component_ptr->isHierarchical = dialog_ptr->isHierarchical->isChecked();
    component_ptr->lambda = dialog_ptr->lambda->value();
    component_ptr->alpha = dialog_ptr->alpha->value();
    dialog_ptr.reset();
}

void mepp_component_Partition_plugin::step1()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Partition_ComponentPtr component_ptr = findOrCreateComponentForViewer<Partition_ComponentPtr, Partition_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->segment();
            component_ptr->set_init(2);

            mw->statusBar()->showMessage(tr("segment mesh...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("segment mesh ... canceled"));
    }
}

void mepp_component_Partition_plugin::step2()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Partition_ComponentPtr component_ptr = findOrCreateComponentForViewer<Partition_ComponentPtr, Partition_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getOpenFileName(viewer, "Load Labelling file",
                                                     QString(), tr("Labelling Files (*.seg *_labels.txt)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->loadLabelling(fname.toStdString()))
            {
                component_ptr->set_init(6);
                mw->statusBar()->showMessage(tr("load Labelling...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("load Labelling...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("load Labelling...canceled"));
        }
    }
}

void mepp_component_Partition_plugin::step3()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Partition_ComponentPtr component_ptr = findOrCreateComponentForViewer<Partition_ComponentPtr, Partition_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getSaveFileName(viewer, "Save Labelling",
                                                     QString(), tr("Labelling Files (*.seg)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->saveLabelling(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("save Labelling...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("save Labelling...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("save Labelling...canceled"));
        }
    }
}

void mepp_component_Partition_plugin::step4()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Partition_ComponentPtr component_ptr = findOrCreateComponentForViewer<Partition_ComponentPtr, Partition_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getSaveFileName(viewer, "Save Partitions",
                                                     QString());
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->saveSubMeshes(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("save paritions...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("save partitions...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("save partitions...canceled"));
        }
    }
}

void mepp_component_Partition_plugin::step5()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Partition_ComponentPtr component_ptr = findOrCreateComponentForViewer<Partition_ComponentPtr, Partition_Component>(viewer, polyhedron_ptr);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        component_ptr->faceLabelToColorMap();
        viewer->recreateListsAndUpdateGL();

        mw->statusBar()->showMessage(tr("face label to color map...done"));
        QApplication::restoreOverrideCursor();
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mepp_component_Partition_plugin, mepp_component_Partition_plugin);
#endif

#endif
