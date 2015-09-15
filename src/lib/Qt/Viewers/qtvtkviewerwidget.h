#ifndef QTVTKVIEWERWIDGET_H
#define QTVTKVIEWERWIDGET_H

#include <QWidget>

#include "ui_qtvtkviewerwidget.h"

#include "qtvtkviewer.h"

#include "vtkRenderer.h"
#include <vtkLookupTable.h>

namespace Ui {
    class QtVTKViewerWidget;
}

class QtVTKViewerWidget : public QWidget
{
    Q_OBJECT

public:
    QtVTKViewerWidget(QWidget *parent = 0);
    ~QtVTKViewerWidget();

    vtkRenderer* getRenderer();

    void setImage(vtkImageData* img);

    void SetLookupTable(vtkLookupTable *lookuptable);    

    void SetOutputFormatToRGBA();

private slots:
    void on_checkBoxShowPlanes_clicked();

    void on_checkBoxShowImageActor_clicked(bool checked);

    void on_horizontalSliderX_valueChanged(int value);

    void on_horizontalSliderY_valueChanged(int value);

    void on_horizontalSliderZ_valueChanged(int value);

private:
    Ui::QtVTKViewerWidget *ui;
    bool _firstset;

};

#endif // QTVTKVIEWERWIDGET_H
