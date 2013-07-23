///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#define BUILD_component_Skeleton_Extraction

#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#include "mepp_component_Skeleton_Extraction_plugin.hxx"

#include <QObject>
#include <QAction>
#include <QApplication>
#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "dialSettings_Skeleton_Extraction.hxx"
#include "Skeleton_Extraction_Component.h"
typedef boost::shared_ptr<Skeleton_Extraction_Component> Skeleton_Extraction_ComponentPtr;
typedef boost::shared_ptr<SettingsDialog_Skeleton_Extraction> SettingsDialogPtr;

SettingsDialogPtr create_dialog_settings(Skeleton_Extraction_ComponentPtr component_ptr)
{
    SettingsDialogPtr dialog_ptr = SettingsDialogPtr(new SettingsDialog_Skeleton_Extraction);
    dialog_ptr->wl0->setValue(component_ptr->wl0);
    dialog_ptr->wh0->setValue(component_ptr->wh0);
    dialog_ptr->sl->setValue(component_ptr->sl);
    dialog_ptr->maxiter->setValue(component_ptr->maxiter);
    dialog_ptr->shapeCost->setValue(component_ptr->wShapeCost);
    dialog_ptr->samplingCost->setValue(component_ptr->wSamplingCost);
    dialog_ptr->useCentroid->setChecked(component_ptr->useCentroid);
    
    std::cout << "Wl0*: " << component_ptr->wl0 << " " << dialog_ptr->wl0->value() << "\n" << std::flush;
    return dialog_ptr;
}

void get_dialog_settings(SettingsDialogPtr dialog_ptr, Skeleton_Extraction_ComponentPtr component_ptr)
{
    component_ptr->wl0 = dialog_ptr->wl0->value();
    component_ptr->wh0 = dialog_ptr->wh0->value();
    component_ptr->sl = dialog_ptr->sl->value();
    component_ptr->maxiter = dialog_ptr->maxiter->value();
    component_ptr->wShapeCost = dialog_ptr->shapeCost->value();
    component_ptr->wSamplingCost = dialog_ptr->samplingCost->value();
    component_ptr->useCentroid = dialog_ptr->useCentroid->isChecked();
    dialog_ptr.reset();
}

void mepp_component_Skeleton_Extraction_plugin::pre_draw()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();

        if (doesExistComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr)) // important !!!
        {
            //std::cout << "pre_draw 1\n" << std::flush;
            Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);
            if (component_ptr->get_init() > 1)
            {
                //std::cout << "pre_draw 2\n" << std::endl;

                glPushMatrix();
                // here your code
                if (component_ptr->get_init() <= 3) draw_contracted_mesh(viewer);
                draw_skeleton(viewer);
                glPopMatrix();
            } else
            {
                std::cout << "status: not ready\n" << std::flush;
            }
        }
    }
}

void mepp_component_Skeleton_Extraction_plugin::draw_contracted_mesh(Viewer* viewer)
{
    PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
    Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);
    PolyhedronPtr pContractedMesh = component_ptr->getContractedMeshPtr();

    if (pContractedMesh != 0)
    {
        GLboolean lightingEnabled;
        GLdouble currentColor[4];

        glGetBooleanv(GL_LIGHTING, &lightingEnabled);
        glGetDoublev(GL_CURRENT_COLOR, currentColor);
        if (lightingEnabled) glDisable(GL_LIGHTING);

        glColor3f(0.0, 0.0, 0.0);
        //pContractedMesh->gl_draw(true, false, false, false, false);
        pContractedMesh->superimpose_edges(false);

        if (lightingEnabled) glEnable(GL_LIGHTING);
        glColor4dv(currentColor);
    }
}

void mepp_component_Skeleton_Extraction_plugin::draw_skeleton(Viewer* viewer)
{
    PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
    Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);
    SkeletonPtr pSkeleton = component_ptr->getSkeletonPtr();

    if (pSkeleton != 0)
    {
        GLboolean lightingEnabled, smoothPointEnabled;
        GLdouble currentColor[4];
        GLdouble lineWidth;
        GLdouble pointSize;

        glGetBooleanv(GL_LIGHTING, &lightingEnabled);
        glGetBooleanv(GL_POINT_SMOOTH, &smoothPointEnabled);
        glGetDoublev(GL_LINE_WIDTH, &lineWidth);
        glGetDoublev(GL_POINT_SIZE, &pointSize);
        glGetDoublev(GL_CURRENT_COLOR, currentColor);

        glLineWidth(3.0);
        glPointSize(6.0);
        if (lightingEnabled) glDisable(GL_LIGHTING);
        if (!smoothPointEnabled) glEnable( GL_POINT_SMOOTH );
        if (pSkeleton->edges.size() > 0)
        {
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINES);
            for (unsigned int i=0; i<pSkeleton->edges.size(); ++i)
            {
                Skeleton::Vec3 vi = pSkeleton->verts[pSkeleton->edges[i].first];
                Skeleton::Vec3 vj = pSkeleton->verts[pSkeleton->edges[i].second];
                glVertex3d(vi.x(), vi.y(), vi.z());
                glVertex3d(vj.x(), vj.y(), vj.z());
            }
            glEnd();
        }
        if (pSkeleton->verts.size() > 0)
        {
            glBegin(GL_POINTS);
            for (unsigned int i=0; i<pSkeleton->verts.size(); ++i)
            {
                Skeleton::Vec3 vi = pSkeleton->verts[i];
                glVertex3d(vi.x(), vi.y(), vi.z());
            }
            glEnd();
        }
        if (lightingEnabled) glEnable(GL_LIGHTING);
        if (!smoothPointEnabled) glDisable( GL_POINT_SMOOTH );
        glColor4dv(currentColor);
        glLineWidth(5.0);
        glPointSize(6.0);
    }
}

void mepp_component_Skeleton_Extraction_plugin::step1()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->contract();
            component_ptr->set_init(2);

            mw->statusBar()->showMessage(tr("Contract mesh...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Contract mesh ... canceled"));
    }
}

void mepp_component_Skeleton_Extraction_plugin::step2()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->simplify();
            component_ptr->set_init(3);

            mw->statusBar()->showMessage(tr("Simplify contracted mesh...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Simplify contracted mesh...canceled"));
    }
}

void mepp_component_Skeleton_Extraction_plugin::step3()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->refineEmbedding();
            component_ptr->set_init(4);

            mw->statusBar()->showMessage(tr("Refine skeleton embeddding...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("Refine skeleton embeddding...canceled"));
    }
}

void mepp_component_Skeleton_Extraction_plugin::step4()
{
    // active viewer
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        SettingsDialogPtr dial_ptr = create_dialog_settings(component_ptr);
        if (dial_ptr->exec() == QDialog::Accepted)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            get_dialog_settings(dial_ptr, component_ptr);

            component_ptr->extractSkeleton();
            component_ptr->set_init(5);

            mw->statusBar()->showMessage(tr("extract skeleton...done"));
            viewer->recreateListsAndUpdateGL();
            QApplication::restoreOverrideCursor();
            return;
        }
        mw->statusBar()->showMessage(tr("extract skeleton...canceled"));
    }
}

void mepp_component_Skeleton_Extraction_plugin::step5()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getOpenFileName(viewer, "Load a skeleton file",
                                                     QString(), tr("Skeleton Files (*.skel)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->loadSkeleton(fname.toStdString()))
            {
                component_ptr->set_init(6);
                mw->statusBar()->showMessage(tr("load skeleton...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("load skeleton...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("load skeleton...canceled"));
        }
    }
}

void mepp_component_Skeleton_Extraction_plugin::step6()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        QString fname = QFileDialog::getSaveFileName(viewer, "Save skeleton",
                                                     QString(), tr("Skeleton Files (*.skel)"));
        if (fname.size() > 0)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (component_ptr->saveSkeleton(fname.toStdString()))
            {
                mw->statusBar()->showMessage(tr("save skeleton...done"));
                viewer->recreateListsAndUpdateGL();
            } else
            {
                mw->statusBar()->showMessage(tr("save skeleton...failed"));
            }
            QApplication::restoreOverrideCursor();
        } else
        {
            mw->statusBar()->showMessage(tr("save skeleton...canceled"));
        }
    }
}

void mepp_component_Skeleton_Extraction_plugin::step7()
{
    if (mw->activeMdiChild() != 0)
    {
        Viewer* viewer = (Viewer *)mw->activeMdiChild();
        PolyhedronPtr polyhedron_ptr = viewer->getScenePtr()->get_polyhedron();
        Skeleton_Extraction_ComponentPtr component_ptr = findOrCreateComponentForViewer<Skeleton_Extraction_ComponentPtr, Skeleton_Extraction_Component>(viewer, polyhedron_ptr);

        QApplication::setOverrideCursor(Qt::WaitCursor);

        component_ptr->set_init(8);
        component_ptr->vertexThicknessToColorMap();
        viewer->recreateListsAndUpdateGL();

        mw->statusBar()->showMessage(tr("thickness to vertex color map...done"));
        QApplication::restoreOverrideCursor();
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mepp_component_Skeleton_Extraction_plugin, mepp_component_Skeleton_Extraction_plugin);
#endif

#endif
