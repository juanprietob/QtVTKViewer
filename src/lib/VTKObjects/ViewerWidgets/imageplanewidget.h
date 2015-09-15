#ifndef IMAGEPLANEWIDGET_H
#define IMAGEPLANEWIDGET_H


#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageData.h"
#include "vtkColorTransferFunction.h"
#include "vtkImageViewer2.h"
#include "vtkSmartPointer.h"

class ImagePlaneWidget
{
public:
    ImagePlaneWidget();

    virtual void setImage(vtkImageData* img);

    virtual void SetLookupTable(vtkLookupTable *lookuptable);

    virtual void SetColorTable(vtkColorTransferFunction *lookuptable);

    virtual void showPlanes(bool show);    

    virtual bool showImageActors(bool show);

    void setSliceXImageActor(int value);
    void setSliceYImageActor(int value);
    void setSliceZImageActor(int value);

protected:

    vtkImageData* _img;
    vtkImagePlaneWidget* _xwidget;
    vtkImagePlaneWidget* _ywidget;
    vtkImagePlaneWidget* _zwidget;
    vtkImageActor * _imageactorx;
    vtkImageActor* _imageactory;
    vtkImageActor* _imageactorz;

    int xslice;
    int yslice;
    int zslice;

    void initialize(vtkRenderWindowInteractor* interactor);

    void initializeImageActors();

private:
    void invariant();

    vtkRenderWindowInteractor* m_Interactor;

};

#endif // IMAGEPLANEWIDGET_H
