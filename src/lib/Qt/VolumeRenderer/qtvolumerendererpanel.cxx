#include "qtvolumerendererpanel.h"
#include "ui_qtvolumerendererpanel.h"
#include "qmessagebox.h"

using namespace std;

QtVolumeRendererPanel::QtVolumeRendererPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtVolumeRendererPanel)
{
    ui->setupUi(this);    
}

QtVolumeRendererPanel::~QtVolumeRendererPanel()
{
    delete ui;
}

void QtVolumeRendererPanel::on_checkBoxShowVolume_clicked(bool checked)
{
    try{
        ShowVolume(checked);
        //JCP 27/09/12
        //on_opacitySlider_valueChanged(this->ui->opacitySlider->value());
    }catch(char * e){
        cout<<e<<endl;
        QMessageBox::critical(this, tr("Volume Rendering"), tr(e), QMessageBox::Ok);
    }catch(exception e){
        cout<<e.what()<<endl;
        QMessageBox::critical(this, tr("Volume Rendering"), tr(e.what()), QMessageBox::Ok);
    }
}


void QtVolumeRendererPanel::on_opacitySlider_valueChanged(int value)
{
    try{        
        this->ui->lineEditOpacity->setText(QString::number(value));        

        double* range = ImageData->GetScalarRange();
        vector<double> greylevel;
        greylevel.push_back(0);        
        greylevel.push_back(1);
        greylevel.push_back(range[1]);

        vector<double> vectvalue;
        vectvalue.push_back(0);
        vectvalue.push_back(value/100.0);
        vectvalue.push_back(value/100.0);

        OpacityChanged(greylevel, vectvalue);
    }catch(char * e){
        cout<<e<<endl;
        QMessageBox::critical(this, tr("Volume Rendering"), tr(e), QMessageBox::Ok);
    }
}

void QtVolumeRendererPanel::on_checkBoxBoundingBox_clicked(bool checked)
{
    try{
        BoundingBoxChanged(checked);
    }catch(char * e){
        cout<<e<<endl;
        QMessageBox::critical(this, tr("Volume Rendering"), tr(e), QMessageBox::Ok);
    }
}

void QtVolumeRendererPanel::on_radioButtonMIP_clicked(bool checked)
{
    if(checked){
        this->changeCompositeMIPFunction(1);
    }
}

void QtVolumeRendererPanel::on_radioButtonComposite_clicked(bool checked)
{
    if(checked){
        this->changeCompositeMIPFunction(0);
    }
}

void QtVolumeRendererPanel::on_radioButtonLinear_clicked(bool checked)
{
    if(checked){
        this->changeInterpolationType(0);
    }
}

void QtVolumeRendererPanel::on_radioButtonNearest_clicked(bool checked)
{
    if(checked){
        this->changeInterpolationType(1);
    }
}

void QtVolumeRendererPanel::on_radioButtonMinIP_clicked(bool checked)
{
    if(checked){
        this->changeCompositeMIPFunction(2);
    }
}
