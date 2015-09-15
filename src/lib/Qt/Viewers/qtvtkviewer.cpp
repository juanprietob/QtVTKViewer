#include "qtvtkviewer.h"

#include "vtkImageMapToColors.h"

QtVTKViewer::QtVTKViewer(QWidget* parent)
    : QVTKWidget(parent)
{


    vtkRenderer* renderer = vtkRenderer::New();
    this->GetRenderWindow()->AddRenderer(renderer);
    this->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(1,1,1);

    SuperImagePlaneWidget::initialize(this->GetRenderWindow()->GetInteractor());

    this->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();    


    _rayorigin[0] = 0;
    _rayorigin[1] = 0;
    _rayorigin[2] = 0;
    _rayorigin[3] = 0;

    _imageindex[0] = 0;
    _imageindex[1] = 0;
    _imageindex[2] = 0;

}

void QtVTKViewer::mouseMoveEvent(QMouseEvent* event){
    int x=0, y=0;

    QVTKWidget::mouseMoveEvent(event);

    x = event->pos().x();
    y = this->size().height() - event->pos().y();

#ifdef debug
    std::cout<<"X= "<<x<<" Y= "<<y<<std::endl;
#endif


    this->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetDisplayPoint(x, y, 0);
    this->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->DisplayToWorld();
    this->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetWorldPoint(_rayorigin);
#ifdef debug
    std::cout<<"X= "<<x<<" Y= "<<y<<std::endl;
    std::cout<<"X= "<<_rayorigin[0]<<" Y= "<<_rayorigin[1]<<" Z= "<<_rayorigin[2]<<" R= "<<_rayorigin[3]<<std::endl;
#endif





}

void QtVTKViewer::mousePressEvent(QMouseEvent* event){
    QVTKWidget::mousePressEvent(event);
    vtkImagePlaneWidget * temp = 0;
    if(_xwidget->GetCursorDataStatus()==1){
        temp = _xwidget;
    }else if(_ywidget->GetCursorDataStatus()==1){
        temp = _ywidget;
    }else if(_zwidget->GetCursorDataStatus()==1){
        temp = _zwidget;
    }

    if(temp){
        _imageindex[0]=temp->GetCurrentCursorPosition()[0];
        _imageindex[1]=temp->GetCurrentCursorPosition()[1];
        _imageindex[2]=temp->GetCurrentCursorPosition()[2];
#ifdef debug
         std::cout<<"X= "<<_imageindex[0]<<" Y= "<<_imageindex[1]<<" Z= "<<_imageindex[2]<<std::endl;
#endif
    }
}

void QtVTKViewer::SetOutputFormatToRGBA(){
    _xwidget->GetColorMap()->SetOutputFormatToRGBA();
    _ywidget->GetColorMap()->SetOutputFormatToRGBA();
    _zwidget->GetColorMap()->SetOutputFormatToRGBA();
}
