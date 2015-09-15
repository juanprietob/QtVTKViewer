#include "qtsurfacerenderer.h"
#include "ui_qtsurfacerenderer.h"


#include <qtsurfacerendererpanel.h>

QtSurfaceRenderer::QtSurfaceRenderer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtSurfaceRenderer)
{
    ui->setupUi(this);

    this->ui->tabWidgetSurface->removeTab(1);
    this->ui->tabWidgetSurface->removeTab(0);


    QtSurfaceRendererPanel* surfacepanel = new QtSurfaceRendererPanel(this);
    this->ui->tabWidgetSurface->addTab(surfacepanel, QString("Surface Rendering"));

    Renderer = 0;
    Interactor = 0;
}

QtSurfaceRenderer::~QtSurfaceRenderer()
{
    delete ui;
}

void QtSurfaceRenderer::SetImageData(vtkImageData* img){
    this->ui->tabWidgetSurface->setCurrentIndex(0);
    QtSurfaceRendererPanel* surfacepanel = (QtSurfaceRendererPanel*)this->ui->tabWidgetSurface->currentWidget();    
    surfacepanel->setInteractor(Interactor);
    surfacepanel->setRenderer(Renderer);
    surfacepanel->setImageData(img);
}

void QtSurfaceRenderer::on_pushbuttonAddTab_clicked()
{

    QtSurfaceRendererPanel* surfacepanel = new QtSurfaceRendererPanel(this);    
    surfacepanel->setRenderer(Renderer);
    surfacepanel->setInteractor(Interactor);

    this->ui->tabWidgetSurface->addTab(surfacepanel, QString("Surface Rendering"));
}

void QtSurfaceRenderer::on_tabWidgetSurface_tabCloseRequested(int index)
{
    if(index != 0){

        QtSurfaceRendererPanel* surfacepanel = (QtSurfaceRendererPanel*)this->ui->tabWidgetSurface->currentWidget();
        surfacepanel->enableBoundingBox(0, false);
        surfacepanel->deleteActor(0);

        this->ui->tabWidgetSurface->removeTab(index);
    }
}

