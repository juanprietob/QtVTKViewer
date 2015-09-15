
#include "volumerenderermanagerdata.h"

/********************************************************************************************
** Start of data viewmanagerData
*********************************************************************************************/

#include "vtkStripper.h"
#include "boxSurfaceObserver.h"
#include "vtkProperty.h"

#include <vtkVolumeRayCastMapper.h>
#include "vtkGPUVolumeRayCastMapper.h"

using namespace std;

VolumeRendererManagerData::VolumeRendererManagerData(vtkImageData* vol, bool usegpu){
    Initialize(vol, "", usegpu);
}

VolumeRendererManagerData::VolumeRendererManagerData(vtkImageData* vol, std::string dataname){

    Initialize(vol, dataname);


}

VolumeRendererManagerData::VolumeRendererManagerData(vtkImageData* vol, vtkRenderer* render, std::string dataname){

    Initialize(vol, dataname);
    render->AddActor(this->getProp3D());

}

void VolumeRendererManagerData::Initialize(vtkImageData* vol, std::string dataname, bool usegpu){
    _id = 0;
    _vol = vol;
    _dataname = dataname;


    _volumePlanes  = vtkPlanes::New();

    _volumeProperty = vtkVolumeProperty::New();
    _volumeProperty->SetInterpolationTypeToLinear();
    //_volumeProperty->ShadeOn();
    _volumeProperty->DisableGradientOpacityOn();    
    _tfun = vtkPiecewiseFunction::New();
    _volumeProperty->SetScalarOpacity(_tfun );
    _ctfun = vtkColorTransferFunction::New();

    if(_vol->GetNumberOfScalarComponents() == 1 ){

        _ctfun->AddRGBPoint(vol->GetScalarRange()[0], 0, 0 , 0);
        _ctfun->AddRGBPoint(vol->GetScalarRange()[1], 1, 1 , 1);
        _ctfun->Build();
        _volumeProperty->SetColor(_ctfun);
    }

    _newvol = vtkVolume::New();
    _newvol->SetProperty(_volumeProperty );    


    _compositeFunction = 0;
    _MIPFunction = 0;

    BoxWidget = 0;

/* EED9Juin2011
    if(usegpu && _vol->GetDataDimension() > 2){
        _volumeMappergpu = vtkGPUVolumeRayCastMapper::New();
        _volumeMappergpu->SetClippingPlanes( _volumePlanes );
        _volumeMappergpu->AutoAdjustSampleDistancesOn();

        _newvol->SetMapper(_volumeMappergpu );

        _volumeMappergpu->SetInput( _vol );
        _volumeMappergpu->Update();
    }else{
        _compositeFunction = vtkVolumeRayCastCompositeFunction::New();
        _MIPFunction = vtkVolumeRayCastMIPFunction::New();

        _volumeMapper = vtkVolumeRayCastMapper::New();
        _volumeMapper->SetVolumeRayCastFunction(_compositeFunction);
        _volumeMapper->SetClippingPlanes( _volumePlanes );
        _volumeMapper->AutoAdjustSampleDistancesOn();

        _newvol->SetMapper(_volumeMapper );

        _volumeMapper->SetInput( _vol );
        _volumeMapper->Update();
    }
*/

       VolumeMapper = 0;
       vtkGPUVolumeRayCastMapper * volumeMappergpu = vtkGPUVolumeRayCastMapper::New();
       volumeMappergpu->AutoAdjustSampleDistancesOn();
       VolumeMapper = volumeMappergpu;

        VolumeMapper->SetClippingPlanes( _volumePlanes );
        _newvol->SetMapper(VolumeMapper );
        VolumeMapper->SetInputData( _vol );
        VolumeMapper->Update();
        _newvol->Update();
}


VolumeRendererManagerData::~VolumeRendererManagerData()
{

        _tfun->Delete();
        _ctfun->Delete();
        _volumePlanes->Delete();
        _volumeProperty->Delete();
        _newvol->Delete();

        if(_compositeFunction)
            _compositeFunction->Delete();
        if(_MIPFunction)
            _MIPFunction->Delete();
        if(VolumeMapper)
            VolumeMapper->Delete();

        if(BoxWidget){
            DisableBoundingBox();
        }

}
void VolumeRendererManagerData::SetIndependentComponents(bool independent){

    if(!independent){
        _volumeProperty->IndependentComponentsOff();
    }else{
        _volumeProperty->IndependentComponentsOn();
    }
}

void VolumeRendererManagerData::EnableBoundingBox(vtkRenderWindowInteractor* interactor)
{
//EED9Juin2011    if(_volumeMappergpu){

    if(!BoxWidget){
        BoxWidget = vtkBoxWidget::New();
        BoxWidget->SetInteractor( interactor );
        BoxWidget->SetPlaceFactor(1.01);

        BoxWidget->SetInputData( _vol );
        BoxWidget->InsideOutOn();
        BoxWidget->PlaceWidget();


        vtkBoxWidgetCallback *callback = vtkBoxWidgetCallback::New();


        callback->SetMapper(VolumeMapper);


        BoxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
        callback->Delete();

        BoxWidget->EnabledOn();
        BoxWidget->GetSelectedFaceProperty()->SetOpacity(0.0);

        cout<<"JPRG::VolumeRendererManagerData::EnableBoundingBox::CREATE"<<endl;

    }else{

        BoxWidget->EnabledOn();

        cout<<"JPRG::VolumeRendererManagerData::EnableBoundingBox"<<endl;

    }
//EED9Juin2011  }
}

void VolumeRendererManagerData::DisableBoundingBox(){

    if(BoxWidget){
        BoxWidget->EnabledOff();
        //BoxWidget->Delete();
        //BoxWidget = 0;

        cout<<"JPRG::VolumeRendererManagerData::DisableBoundingBox"<<endl;


    }
}

/**
**	Set Volume Color
**/
void VolumeRendererManagerData::setVolumeColor(std::vector<double>& greylevelcolors,
                                                                std::vector<double>& red,
                                                                std::vector<double>& green,
                                                                std::vector<double>& blue)
{

        _ctfun->RemoveAllPoints();
        for(unsigned int i = 0; i < greylevelcolors.size();i++){
                _ctfun->AddRGBPoint(greylevelcolors[i], red[i],green[i], blue[i]);
                                //std::cout<<"VolumeRendererManagerData::setVolumeColor "<<greylevelcolors[i]<<" "<<red[i]<<" "<<green[i]<<" "<<blue[i]<<std::endl;
        }
        _newvol->Update();
}

/**
**	Volume Opacity
**/
void VolumeRendererManagerData::setVolumeOpacity(std::vector<double> greylevel,std::vector<double> value){

        _tfun->RemoveAllPoints();
        for(unsigned int i = 0; i < greylevel.size();i++){
                _tfun->AddPoint(greylevel[i], value[i]);
                std::cout<<"VolumeRendererManagerData::setVolumeOpacity "<<greylevel[i]<<" "<< value[i]<<std::endl;
        }
    _newvol->Update();
}

/**
**	Volume Opacity
**/
void VolumeRendererManagerData::setVolumeOpacity(std::vector<double> greylevel,double value){
    std::vector<double> valuevector;
    for(unsigned i = 0; i < greylevel.size(); i++){
        valuevector.push_back(value);
    }
    setVolumeOpacity(greylevel, valuevector);
}

/**
**	Check if the variables are setted correctly
**/
void VolumeRendererManagerData::checkInvariant()throw (char *){
        if(!_compositeFunction){
                throw "No composite function initialized";
        }
        if(!_MIPFunction){
                throw "No MIP function initialized";
        }
        if(!VolumeMapper){
                throw "No volume mapper initialized";
        }
}

/**
**	Check if the variables are setted correctly
**/
void VolumeRendererManagerData::Update(){
         _newvol->Update();
}
/**
**	get the prop3D
**/
vtkProp3D* VolumeRendererManagerData::getProp3D(){
        return this->_newvol;
}
/**
**	return the id from the daat
**/
int VolumeRendererManagerData::getId(){
        return _id;
}
/**
**	set data id
**/
void VolumeRendererManagerData::setId(int propid){
        _id = propid;
}

/**
**	Get the filanme
**/
std::string VolumeRendererManagerData::getDataname(){
        return _dataname;
}
/**
** Set the filanme
**/
void VolumeRendererManagerData::setDataname(std::string dataname){
        _dataname = dataname;
}

void VolumeRendererManagerData::changeCompositeMIPFunction(int function){
        //checkInvariant();
        if(VolumeMapper){
#if VTK_MAJOR_VERSION >= 5
    #if VTK_MINOR_VERSION >= 6
            if(dynamic_cast<vtkVolumeRayCastMapper*>(VolumeMapper)){
                vtkVolumeRayCastMapper* volumemapper = dynamic_cast<vtkVolumeRayCastMapper*>(VolumeMapper);
                if(function == 0){
                        volumemapper->SetVolumeRayCastFunction(_compositeFunction);
                }else{
                        volumemapper->SetVolumeRayCastFunction(_MIPFunction);
                }
            }else if(dynamic_cast<vtkGPUVolumeRayCastMapper*>(VolumeMapper)){
                vtkGPUVolumeRayCastMapper* volumemapper = dynamic_cast<vtkGPUVolumeRayCastMapper*>(VolumeMapper);
                if(function == 0){
                    volumemapper->SetBlendModeToComposite();
                }else if(function == 1){
                    volumemapper->SetBlendModeToMaximumIntensity();
                }else if(function == 2){
                    volumemapper->SetBlendModeToMinimumIntensity();
                }
            }
    #endif
#else
            vtkGPUVolumeRayCastMapper* volumemapper = dynamic_cast<vtkGPUVolumeRayCastMapper*>(VolumeMapper);
            if(function == 0){
                volumemapper->SetBlendModeToComposite();
            }else if(function == 1){
                volumemapper->SetBlendModeToMaximumIntensity();
            }else if(function == 2){
                volumemapper->SetBlendModeToMinimumIntensity();
            }
#endif
        }

}

void VolumeRendererManagerData::SetLookupTable(vtkLookupTable* lookuptable){
    if(lookuptable){
         _ctfun->RemoveAllPoints();
         vtkColorTransferFunction* colort = (vtkColorTransferFunction*)lookuptable;
         for(int i = 0; i < colort->GetSize(); i++){
             double val[6];
             colort->GetNodeValue(i, val);
             cout<< "JPRG::VolumeRendererManagerData::SetLookupTable::"<<val[0]<<" "<< val[1]<<" "<<val[2]<<" "<< val[3]<<endl;
             _ctfun->AddRGBPoint(val[0], val[1], val[2], val[3]);
         }
         _newvol->Update();
    }

}

void VolumeRendererManagerData::changeInterpolationType(int type){
    //checkInvariant();
    if(type == 0){
        _volumeProperty->SetInterpolationTypeToLinear();
    }else if(type == 1){
        _volumeProperty->SetInterpolationTypeToNearest();
    }
}

void VolumeRendererManagerData::SetColorTransferFunction(int i, vtkColorTransferFunction* colorf){

    _volumeProperty->SetColor(i, colorf);

}
