#include "qtvtkviewerwidget.h"
#include "ui_qtvtkviewerwidget.h"



QtVTKViewerWidget::QtVTKViewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtVTKViewerWidget)
{
    ui->setupUi(this);

    ui->m_VolumeRender->SetRenderer(this->getRenderer());
    ui->m_VolumeRender->SetInteractor(ui->_qtvtkviewer->GetRenderWindow()->GetInteractor());

    ui->m_SurfaceRender->SetRenderer(this->getRenderer());
    ui->m_SurfaceRender->SetInteractor(ui->_qtvtkviewer->GetRenderWindow()->GetInteractor());

}

QtVTKViewerWidget::~QtVTKViewerWidget()
{

}

vtkRenderer* QtVTKViewerWidget::getRenderer(){
    return ui->_qtvtkviewer->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
}

void QtVTKViewerWidget::setImage(vtkImageData* img){


    ui->_qtvtkviewer->setImage(img);
    ui->m_VolumeRender->SetImageData(img);
    ui->m_SurfaceRender->SetImageData(img);

    this->getRenderer()->ResetCamera();

    int extent[6];
    img->GetExtent(extent);


    ui->horizontalSliderX->setRange(extent[0], extent[1]);
    ui->horizontalSliderY->setRange(extent[2], extent[3]);
    ui->horizontalSliderZ->setRange(extent[4], extent[5]);

}

void QtVTKViewerWidget::SetLookupTable(vtkLookupTable *lookuptable){

    ui->_qtvtkviewer->SetLookupTable(lookuptable);
    ui->m_VolumeRender->SetLookUpTable(lookuptable);
}

void QtVTKViewerWidget::on_checkBoxShowPlanes_clicked(){

    ui->_qtvtkviewer->showPlanes(ui->checkBoxShowPlanes->isChecked());
}

void QtVTKViewerWidget::SetOutputFormatToRGBA(){
    ui->_qtvtkviewer->SetOutputFormatToRGBA();
}

void QtVTKViewerWidget::on_checkBoxShowImageActor_clicked(bool checked)
{
    if(!(ui->_qtvtkviewer->showImageActors(checked))){
        ui->checkBoxShowImageActor->setCheckState(Qt::Unchecked);
    }
}

void QtVTKViewerWidget::on_horizontalSliderX_valueChanged(int value)
{

    ui->lineEditX->setText(QString::number(value));
    ui->_qtvtkviewer->setSliceXImageActor(value);

}

void QtVTKViewerWidget::on_horizontalSliderY_valueChanged(int value)
{
    ui->lineEditY->setText(QString::number(value));
    ui->_qtvtkviewer->setSliceYImageActor(value);
}

void QtVTKViewerWidget::on_horizontalSliderZ_valueChanged(int value)
{
    ui->lineEditZ->setText(QString::number(value));
    ui->_qtvtkviewer->setSliceZImageActor(value);
}
