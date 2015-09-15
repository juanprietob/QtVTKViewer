#include "volumerendererdata.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(VolumeRendererData, "$Revision: 1.8 $");
vtkStandardNewMacro(VolumeRendererData);

VolumeRendererData::VolumeRendererData()
{
    VolRendManager = 0;
    ImageData = 0;
    Renderer = 0;
    Interactor = 0;
    LookUpTable = 0;
    //ImageDataSeparateComponents = false;
}


void VolumeRendererData::ShowVolume(bool checked){

    try{
        if(checked){
                invariant();
                cout<<"JPRG::VolumeRendererData::ShowVolume"<<endl;
                if(!VolRendManager){
                    VolRendManager = new VolumeRendererManager();
                    VolRendManager->addVolume(ImageData, Interactor);
                }
                if(LookUpTable)
                    VolRendManager->SetLookupTable(LookUpTable);
                vector< vtkProp3D* > props = VolRendManager->getProps3D();
                for(unsigned i = 0; i < props.size(); i++){
                    Renderer->AddActor(props[i]);
                }

        }else{
            vector< vtkProp3D* > props = VolRendManager->getProps3D();
            for(unsigned i = 0; i < props.size(); i++){
                Renderer->RemoveActor(props[i]);
            }
            delete VolRendManager;
            VolRendManager = 0;
        }
    }catch(char* e){
        cout<<"Exception in: "<<e<<endl;
    }catch(exception e){
        cout<<"Exception in: "<<e.what()<<endl;
    }

}

void VolumeRendererData::invariant(){
    if(!ImageData){
        throw "Image data not set";
    }
    if(!Renderer){
        throw "renderer not initialized";
    }
}

void VolumeRendererData::OpacityChanged(vector<double> greyvalues, vector<double> values){
    //invariant();
    if(VolRendManager){
        ShowVolume(true);
		VolRendManager->setVolumeOpacity(greyvalues,values);
    }
}

void  VolumeRendererData::BoundingBoxChanged(bool checked){
   // invariant();

    if(VolRendManager==NULL)
    {
         ShowVolume(true);
    }


    if(VolRendManager && Interactor){
        if(checked){
            VolRendManager->EnableBoundingBox(Interactor);
        }else{
            VolRendManager->DisableBoundingBox();
        }
    }
}

void VolumeRendererData::changeCompositeMIPFunction(int function){
    invariant();
    if(VolRendManager)
        VolRendManager->changeCompositeMIPFunction(0, function);
}

/**
  changes the interoplation type of the volume
  type = 0 linear
  type = 1 nearest
   */
void VolumeRendererData::changeInterpolationType(int type){
    invariant();
    if(VolRendManager)
        VolRendManager->changeInterpolationType(type);
}
