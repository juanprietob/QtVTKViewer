#include "qtsurfacerendererpanel.h"
#include "ui_qtsurfacerendererpanel.h"

#include "qfiledialog.h"

#include "qcolordialog.h"
#include "qmessagebox.h"

#include "Color.xpm"
#include "Save.xpm"
#include "OpenImage.xpm"

QtSurfaceRendererPanel::QtSurfaceRendererPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtSurfaceRendererPanel)
{
    ui->setupUi(this);


    this->ui->pushButtonColorChooser->setIcon(QIcon(Color_xpm));
    this->ui->pushButtonSave->setIcon(QIcon(Save_xpm));
    this->ui->pushButtonOpen->setIcon(QIcon(OpenImage_xpm));
}

QtSurfaceRendererPanel::~QtSurfaceRendererPanel()
{   
    delete ui;
}
void QtSurfaceRendererPanel::setImageData(vtkImageData* img){
    double *range  =img->GetScalarRange();

    this->ui->horizontalSliderMinIso->setRange(range[0], range[1]);

    this->ui->horizontalSliderMaxIso->setRange(range[0], range[1]);

    wxMaracasSurfaceRenderingManager::setImageData(img);
}

void QtSurfaceRendererPanel::on_OpacitySlider_valueChanged(int value)
{
    try{

        this->ui->lineEditOpacity->setText(QString::number(value));


        this->changeOpacity(0, value);
    }catch(char * e){
        cout<<e<<endl;
        QMessageBox::critical(this, tr("Volume Rendering"), tr(e), QMessageBox::Ok);
    }
}

void QtSurfaceRendererPanel::on_pushButtonColorChooser_clicked()
{
    QColorDialog* qcolor = new QColorDialog(this);



    if(qcolor->exec()){

        QColor color = qcolor->selectedColor();

        int r = 0, g = 0, b = 0;
        color.getRgb(&r, &g, &b);

        try{

            this->changeColor(0, r/255.0, g/255.0 ,b/255.0);
        }catch(char * e){
            cout<<e<<endl;
            QMessageBox::critical(this, tr("Surface Rendering"), tr(e), QMessageBox::Ok);
        }
    }


}

void QtSurfaceRendererPanel::on_checkBox_clicked(bool checked)
{
    this->addRemoveActor(0, checked);
    if(!checked){
        this->ui->checkBoxBoundingBox->setCheckState(Qt::Unchecked);
        this->on_checkBoxBoundingBox_clicked(false);
    }
}

void QtSurfaceRendererPanel::on_horizontalSliderMaxIso_valueChanged(int value)
{

    this->ui->lineEditMaxIso->setText(QString::number(value));
}

void QtSurfaceRendererPanel::on_horizontalSliderMinIso_valueChanged(int value)
{
    this->ui->lineEditMinIso->setText(QString::number(value));
}

void QtSurfaceRendererPanel::on_horizontalSliderMaxIso_sliderReleased()
{
    onIsoValueChanged();
}

void QtSurfaceRendererPanel::on_horizontalSliderMinIso_sliderReleased()
{
    onIsoValueChanged();
}

void QtSurfaceRendererPanel::onIsoValueChanged(){
    this->changeIsoValue(0, this->ui->horizontalSliderMinIso->value(), this->ui->horizontalSliderMaxIso->value());
}

void QtSurfaceRendererPanel::on_pushButtonSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Mesh File"),
                                                    QDir::currentPath(),
                                                    tr("Mesh files (*.stl *.ply)") );

    if( !filename.isNull() ){
        //filename.append(".stl");
        this->saveProp3DSTL(0, filename.toStdString().c_str());
    }

}

void QtSurfaceRendererPanel::on_checkBoxBoundingBox_clicked(bool checked)
{
    this->enableBoundingBox(0, checked);

}

void QtSurfaceRendererPanel::on_pushButtonOpen_clicked(bool checked)
{

}

void QtSurfaceRendererPanel::on_pushButtonOpen_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Mesh File"), QDir::currentPath(),  tr("Mesh files (*.stl *.ply)"));

    if(!filename.isNull()){
        this->loadProp3DSTL(filename.toStdString().c_str());
    }
}
