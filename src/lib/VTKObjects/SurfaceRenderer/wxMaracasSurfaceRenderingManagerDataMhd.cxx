
#include "wxMaracasSurfaceRenderingManagerDataMhd.h"

#include "vtkStripper.h"


wxMaracasSurfaceRenderingManagerDataMhd::wxMaracasSurfaceRenderingManagerDataMhd(vtkImageData* imagedata, std::string dataname, vtkRenderWindowInteractor*  interactor)
: wxMaracasSurfaceRenderingManagerData(NULL, dataname)
{

	this->setVtkImageData(imagedata);
	_dataname = dataname;
	
	_maxgreylevel = getMaxLevel(imagedata);
	_prop3D=NULL;			

        _tresholdFilter = vtkImageThreshold::New();
        _tresholdFilter->SetInputData(this->_imagedata);
        _tresholdFilter->SetInValue(255);
        _tresholdFilter->SetOutValue(0);


	_cubesFilter = vtkMarchingCubes::New();	
        _cubesFilter->SetInputData(_tresholdFilter->GetOutput());
        //_cubesFilter->ComputeGradientsOn();
        _cubesFilter->ComputeScalarsOn();
        //_cubesFilter->ComputeNormalsOn();
        //_cubesFilter->SetNumberOfContours( 1 );
	_cleanFilter = vtkCleanPolyData::New();		
    _cleanFilter->SetInputData ( _cubesFilter->GetOutput() );

        _smooth = vtkSmoothPolyDataFilter::New();
        _smooth->SetInputData(_cleanFilter->GetOutput());
        _smooth->SetNumberOfIterations(6);
        _smooth->SetRelaxationFactor(0.3);
        //_smooth->FeatureEdgeSmoothingOff();

	_dataMapper = vtkPolyDataMapper::New( );
	_dataMapper->ScalarVisibilityOff( );
	_dataMapper->ImmediateModeRenderingOn();
	vtkActor* dataActor = vtkActor::New();

	//if(_boxWidgetS1){
	if(interactor){
            _boxWidgetS1 = vtkBoxWidget::New();
            _boxWidgetS1->SetInteractor( interactor );
            _boxWidgetS1->SetPlaceFactor(1.25);

            _boxWidgetS1->SetInputData( this->_imagedata );
            _boxWidgetS1->PlaceWidget();
            boxSurfaceObserver* observer = boxSurfaceObserver::New();

            vtkStripper* striper = vtkStripper::New();
            striper->SetInputData( _smooth->GetOutput() );
            //striper->SetInput( _cleanFilter->GetOutput() );

            striper->Update();
            _boxWidgetS1->SetInputData(striper->GetOutput());
            //_boxWidgetS1->PlaceWidget();


            _tissuePlanes  = vtkPlanes::New();

            int x1,x2,y1,y2,z1,z2;
            this->_imagedata->GetExtent(x1,x2,y1,y2,z1,z2);
            _tissuePlanes->SetBounds  (x1,x2,y1,y2,z1,z2);



            _boxWidgetS1->GetPlanes( _tissuePlanes );

            _tissueClipper = vtkClipPolyData::New();
            _tissueClipper->SetInputData( striper->GetOutput() );
            _tissueClipper->SetClipFunction( _tissuePlanes );
            _tissueClipper->InsideOutOn( );
            _dataMapper->SetInputData( _tissueClipper->GetOutput() );
            observer->SetPlanes( _tissuePlanes );
            observer->SetActor( dataActor );
            _boxWidgetS1->AddObserver( vtkCommand::InteractionEvent		 , observer );


            _boxWidgetS1->HandlesOn ();
            _boxWidgetS1->EnabledOff();
	}else{		
                //_dataMapper->SetInput(_cleanFilter->GetOutput());
            _dataMapper->SetInputData(_smooth->GetOutput());
	}
	
	dataActor->SetMapper(_dataMapper);	
        float cr=1,cg=0.5,cb=0.5;
        dataActor->GetProperty()->SetDiffuseColor(1,0.5,0.5   );
        dataActor->GetProperty()->SetSpecular(.3);
        dataActor->GetProperty()->SetSpecularPower(20);

        this->_prop3D = dataActor;
	

	this->changeIsoValue(this->_maxgreylevel);	


}

void wxMaracasSurfaceRenderingManagerDataMhd::enableBoxWidget(bool enable){

    if(_boxWidgetS1){
        if(enable){
            _boxWidgetS1->EnabledOn();
        }else{
            _boxWidgetS1->EnabledOff();
        }
    }else{
        cout<<"box widget not initialized!"<<endl;
    }
}


wxMaracasSurfaceRenderingManagerDataMhd::~wxMaracasSurfaceRenderingManagerDataMhd()
{
	_cubesFilter->Delete();
	_cleanFilter->Delete();
	_dataMapper->Delete();
}
	
void wxMaracasSurfaceRenderingManagerDataMhd::UpdateSurface()
{
	_cubesFilter->Update();    
	_cleanFilter->Update();
	_dataMapper->Update();	
}
/**
**	changes the isovalue in a prop3D
**/
void wxMaracasSurfaceRenderingManagerDataMhd::changeIsoValue(double value){	
		
   
    _tresholdFilter->ThresholdByLower(value);
    _tresholdFilter->Update();

    _cubesFilter->SetValue(0,255);
	_cubesFilter->Update();    
	_cleanFilter->Update();
	_dataMapper->Update();	
	
	
	
}
/**
**	changes the isovalue in a prop3D
**/
void wxMaracasSurfaceRenderingManagerDataMhd::changeIsoValue(double min, double max){

    _tresholdFilter->ThresholdBetween(min, max);
    _tresholdFilter->Update();
    _cubesFilter->SetValue(0,255);
        _cubesFilter->Update();
        _cleanFilter->Update();
        _dataMapper->Update();



}
int wxMaracasSurfaceRenderingManagerDataMhd::getMaxGreyLevel(){
	return _maxgreylevel;
}

/**
	** Get's the max grey level of the image
	**/
int wxMaracasSurfaceRenderingManagerDataMhd::getMaxLevel(vtkImageData* img){

	int ext[6], i, j, k,max=0;
	img->GetExtent(ext);

	for(i = ext[0]; i < ext[1];i++){
		for(j = ext[2]; j < ext[3];j++){
			for(k = ext[4]; k < ext[5];k++){
                unsigned short* ptr = (unsigned short*)img->GetScalarPointer(i,j,k);
				int temp = (int)*ptr;
				if(temp > max){
                    max = temp;
				}
			}
		}
	}
	return max;


}

/**
**	Sets the VTK image data
**/

void wxMaracasSurfaceRenderingManagerDataMhd::setVtkImageData(vtkImageData* imagedata){
	_imagedata = imagedata;
}




