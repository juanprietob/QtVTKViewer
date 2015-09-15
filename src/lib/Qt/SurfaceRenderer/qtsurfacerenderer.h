#ifndef QTSURFACERENDERER_H
#define QTSURFACERENDERER_H

#include <QWidget>
#include "wxMaracasSurfaceRenderingManager.h"

namespace Ui {
    class QtSurfaceRenderer;
}

class QtSurfaceRenderer : public QWidget,public vtkObject
{
    Q_OBJECT

public:
    explicit QtSurfaceRenderer(QWidget *parent = 0);

    ~QtSurfaceRenderer();

    vtkSetMacro(Renderer, vtkRenderer*);
    vtkSetMacro(Interactor, vtkRenderWindowInteractor*);

    void SetImageData(vtkImageData* img);
private slots:
    void on_pushbuttonAddTab_clicked();

    void on_tabWidgetSurface_tabCloseRequested(int index);


private:
    Ui::QtSurfaceRenderer *ui;

    vtkRenderer* Renderer;
    vtkRenderWindowInteractor* Interactor;
};

#endif // QTSURFACERENDERER_H
