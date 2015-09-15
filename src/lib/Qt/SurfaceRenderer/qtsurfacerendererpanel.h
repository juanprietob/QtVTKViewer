#ifndef QTSURFACERENDERERPANEL_H
#define QTSURFACERENDERERPANEL_H

#include <QWidget>

#include "wxMaracasSurfaceRenderingManager.h"
#include "vtkImageData.h"

namespace Ui {
    class QtSurfaceRendererPanel;
}

class QtSurfaceRendererPanel : public QWidget, public wxMaracasSurfaceRenderingManager
{
    Q_OBJECT

public:
    explicit QtSurfaceRendererPanel(QWidget *parent = 0);
    ~QtSurfaceRendererPanel();

    virtual void setImageData(vtkImageData* img);
private slots:    

    void on_OpacitySlider_valueChanged(int value);

    void on_pushButtonColorChooser_clicked();

    void on_checkBox_clicked(bool checked);    

    void on_horizontalSliderMaxIso_valueChanged(int value);

    void on_horizontalSliderMinIso_valueChanged(int value);

    void on_horizontalSliderMaxIso_sliderReleased();

    void on_horizontalSliderMinIso_sliderReleased();

    void on_pushButtonSave_clicked();

    void on_checkBoxBoundingBox_clicked(bool checked);

    void on_pushButtonOpen_clicked(bool checked);

    void on_pushButtonOpen_clicked();


private:
    Ui::QtSurfaceRendererPanel *ui;


    void onIsoValueChanged();
};

#endif // QTSURFACERENDERERPANEL_H
