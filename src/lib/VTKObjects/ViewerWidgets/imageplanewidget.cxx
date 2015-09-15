#include "imageplanewidget.h"

#include "vtkSmartPointer.h"
#include "vector"
#include "vtkImageActor.h"
#include "vtkImageAppendComponents.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkTextProperty.h"
#include "vtkCamera.h"

using namespace std;

ImagePlaneWidget::ImagePlaneWidget()
{
    _img = 0;
    _xwidget = 0;
    _ywidget = 0;
    _zwidget = 0;
    m_Interactor = 0;
    _imageactorx = 0;
    _imageactory = 0;
    _imageactorz = 0;
}

void ImagePlaneWidget::initialize(vtkRenderWindowInteractor* interactor){
    m_Interactor = interactor;
    // Plane widget
    _xwidget = vtkImagePlaneWidget::New( );
    _ywidget = vtkImagePlaneWidget::New( );
    _zwidget = vtkImagePlaneWidget::New( );


    _xwidget->SetInteractor( interactor );
    _ywidget->SetInteractor( interactor );
    _zwidget->SetInteractor( interactor );

}
bool ImagePlaneWidget::showImageActors(bool show){
    if(show && m_Interactor){
        if(!_imageactorx){
            initializeImageActors();
        }
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->AddViewProp(_imageactorx);
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->AddViewProp(_imageactory);
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->AddViewProp(_imageactorz);
        return true;
    }else if(_imageactorx){
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->RemoveViewProp(_imageactorx);
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->RemoveViewProp(_imageactory);
        m_Interactor->GetRenderWindow ()->GetRenderers ()->GetFirstRenderer ()->RemoveViewProp(_imageactorz);
        return true;
    }

    return false;
}

void ImagePlaneWidget::initializeImageActors(){

    int w_ext[6];
    _img->GetExtent(w_ext);
    //cout<<w_ext[0]<<" "<<w_ext[1]<<" "<<w_ext[2]<<" "<<w_ext[3]<<" "<<w_ext[4]<<" "<<w_ext[5]<<" "<<endl;
    _imageactorx = vtkImageActor::New();
    _imageactorx->SetInputData(_img);
    xslice =  (w_ext[4] + w_ext[5])/2;
    _imageactorx->SetDisplayExtent(w_ext[0], w_ext[1], w_ext[2], w_ext[3], xslice, xslice);


    _imageactory = vtkImageActor::New();
    _imageactory->SetInputData(_img);
    yslice =  (w_ext[2] + w_ext[3])/2;
    _imageactory->SetDisplayExtent(w_ext[0], w_ext[1], yslice, yslice, w_ext[4], w_ext[5]);

    _imageactorz = vtkImageActor::New();
    _imageactorz->SetInputData(_img);
    zslice =  (w_ext[0] + w_ext[1])/2;
    _imageactorz->SetDisplayExtent(zslice, zslice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);


}

void ImagePlaneWidget::setImage(vtkImageData* img){
    _img = img;
    this->invariant();

    int *dims = _img->GetDimensions( );
    unsigned int sliceX = dims[0]/2;
    unsigned int sliceY = dims[1]/2;
    unsigned int sliceZ = dims[2]/2;

    // Set input X,Y and Z plane
    _xwidget->Off( );
    _xwidget->SetInputData( _img );
    _xwidget->SetPlaneOrientationToXAxes( );
    _xwidget->SetSliceIndex( sliceX );
    _xwidget->TextureInterpolateOff();
    _xwidget->SetResliceInterpolateToLinear();
    _xwidget->On( );
    _xwidget->DisplayTextOn();
    _xwidget->GetTextProperty()->SetColor(0.3, 0.3, 0.3);

    _ywidget->Off( );
    _ywidget->SetInputData( _img );
    _ywidget->SetPlaneOrientationToYAxes( );
    _ywidget->SetSliceIndex( sliceY );
    _ywidget->TextureInterpolateOff();
    _ywidget->SetResliceInterpolateToLinear();
    _ywidget->DisplayTextOn();
    _ywidget->GetTextProperty()->SetColor(0.3, 0.3, 0.3);
    _ywidget->On( );

    _zwidget->Off( );
    _zwidget->SetInputData( _img );
    _zwidget->SetPlaneOrientationToZAxes( );
    _zwidget->SetSliceIndex( sliceZ );
    _zwidget->TextureInterpolateOn();
    _zwidget->SetResliceInterpolateToLinear();
    _zwidget->DisplayTextOn();
    _zwidget->On( );
    _zwidget->GetTextProperty()->SetColor(0.3, 0.3, 0.3);
}

void ImagePlaneWidget::SetLookupTable(vtkLookupTable *lookuptable){
    this->invariant();
    _xwidget->SetLookupTable(lookuptable);
    _ywidget->SetLookupTable(lookuptable);
    _zwidget->SetLookupTable(lookuptable);
}

void ImagePlaneWidget::SetColorTable(vtkColorTransferFunction *colortransfer){
    SetLookupTable((vtkLookupTable*)colortransfer);
}

void ImagePlaneWidget::showPlanes(bool show){
    this->invariant();
    if(show){
        _xwidget->On();
        _ywidget->On();
        _zwidget->On();
    }else{
        _xwidget->Off();
        _ywidget->Off();
        _zwidget->Off();
    }
}


void ImagePlaneWidget::invariant(){
    if(_img == NULL){
        throw "Image data is not initialized";
    }
    if(!_xwidget ||!_ywidget || !_zwidget){
        throw "The widgets are not initialized";
    }
}

void ImagePlaneWidget::setSliceXImageActor(int value){
    int w_ext[6];
    _img->GetExtent(w_ext);
    xslice = value;
    _imageactorx->SetDisplayExtent(w_ext[0], w_ext[1], w_ext[2], w_ext[3], xslice, xslice);
    m_Interactor->Render();
}
void ImagePlaneWidget::setSliceYImageActor(int value){
    int w_ext[6];
    _img->GetExtent(w_ext);
    yslice = value;
    _imageactory->SetDisplayExtent(w_ext[0], w_ext[1], yslice, yslice, w_ext[4], w_ext[5]);
    m_Interactor->Render();
}
void ImagePlaneWidget::setSliceZImageActor(int value){
    int w_ext[6];
    _img->GetExtent(w_ext);
    zslice = value;
    _imageactorz->SetDisplayExtent(zslice, zslice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
    m_Interactor->Render();
}
