#include "volumerenderermanager.h"

/*=========================================================================

  Program:   wxMaracas
  Module:    $RCSfile: volumerenderermanager.cxx,v $
  Language:  C++
  Date:      $Date: 2011/10/19 16:08:29 $
  Version:   $Revision: 1.3 $

  Copyright: (c) 2002, 2003
  License:

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>

#include "vtkImageLuminance.h"
#include "vtkImageAppendComponents.h"
#include "vtkSmartPointer.h"

#include <map>

using namespace std;

/**
**	Start of the manager class
**/
VolumeRendererManager::VolumeRendererManager(){
        _renderer = NULL;
        _idCount=0;
}
VolumeRendererManager::~VolumeRendererManager(){
    _renderer = 0;
    _idCount=0;
    image = 0;
    for(unsigned i = 0; i < prop3Dvect.size();i++){
        prop3Dvect[i]->Delete();
    }
    prop3Dvect.clear();
}

/**
**	Sets the renderer to manage the prop3D from the view
**/
void VolumeRendererManager::setRenderer(vtkRenderer*  renderer){
        _renderer = renderer;
}

/**
** Gets the renderer which manage the prop3D from the view
**/
vtkRenderer* VolumeRendererManager::getRenderer(){
        return _renderer;
}

/**
** Updates Volume
**/
void VolumeRendererManager::Update(int ppid){
        VolumeRendererManagerData* data = this->getViewData(ppid);
        data->Update();
        _renderer->Render();
}

/**
* @pre   The image can have one or multiple components per voxel, and volume rendering is performed seprately over the
         three of them. If the image has multiple components and the separate components flag is set to false, then
         the vtkImageAppendComponents is used to create a single image.
* @post  The volume rendering is performed over the image vol
* @param vtkImageData* the image volume
* @param bool separatecomponents, if the image has multiple components, then a mapper is used for each of the channels
              if this flag is set to false and the volume has multiple components, vtkImageAppendComponents is used to create
              a single representation of the image.
*/
int VolumeRendererManager::addVolume(vtkImageData* img, vtkRenderWindowInteractor* interactor, bool independentcomponents){
    if(img->GetNumberOfScalarComponents() > 1 && !independentcomponents){


        image = img;


        vtkSmartPointer< vtkImageLuminance > luminance = vtkSmartPointer< vtkImageLuminance >::New();
        luminance->SetInputData(img);
        luminance->Update();

        vtkSmartPointer< vtkImageAppendComponents > append = vtkSmartPointer< vtkImageAppendComponents >::New();
        append->AddInputData(img);
        append->AddInputData(luminance->GetOutput());
        append->Update();



        VolumeRendererManagerData* data = new VolumeRendererManagerData(append->GetOutput(), true);
        data->SetIndependentComponents(independentcomponents);


        prop3Dvect.push_back(data);

        data->setId(_idCount);
        _idCount++;

        return _idCount-1;


        /*image = img;

        vector< vtkImageData* > vectimg;
        GetImages(img, vectimg);
        vtkBoxWidget* boxw = 0;
        for(unsigned i = 0; i < vectimg.size(); i++){
            VolumeRendererManagerData* data = new VolumeRendererManagerData(vectimg[i], "");

            vtkColorTransferFunction* colorf = data->GetColorFunction();
            colorf->RemoveAllPoints();
            double r = 0, g = 0, b = 0;
            for(unsigned j = 0; j < 255; j++){

                if(i == 0){
                    r = j/255.0;
                    g = 0;
                    b = 0;
                }else if(i == 1){
                    r = 0;
                    g = j/255.0;
                    b = 0;
                }else if(i == 2){
                    r = 0;
                    g = 0;
                    b = j/255.0;
                }

                colorf->AddRGBPoint(j, r, g, b);
            }

            prop3Dvect.push_back(data);

            data->setId(_idCount);
            _idCount++;

            if(!boxw){
                EnableBoundingBox(interactor, data->getId());
                DisableBoundingBox(data->getId());
                boxw = data->GetBoxWidget();
            }else{
                data->SetBoxWidget(boxw);
            }
        }

        boxw->RemoveAllObservers();

        vtkBoxWidgetCallback *callback = vtkBoxWidgetCallback::New();

        for(unsigned i = 0; i < prop3Dvect.size(); i++){
            VolumeRendererManagerData* data = prop3Dvect[i];
            callback->AddMapper(data->GetVolumeMapper());
        }

        boxw->AddObserver(vtkCommand::InteractionEvent, callback);
        callback->Delete();

        return _idCount-1;*/

        /*vtkImageData* imgshort = 0;
        imgshort = vtkImageData::New();
        imgshort->SetNumberOfScalarComponents(1);
        imgshort->SetExtent(img->GetExtent());
        imgshort->SetSpacing(img->GetSpacing());
        imgshort->SetOrigin(img->GetOrigin());
        imgshort->SetScalarTypeToUnsignedShort();
        imgshort->AllocateScalars();
        GetImageDouble(img, imgshort);

        VolumeRendererManagerData* data = new VolumeRendererManagerData(imgshort, "");

        vtkColorTransferFunction* colorf = data->GetColorFunction();
        colorf->RemoveAllPoints();

        map< unsigned short, vector< double > > colormap;

        int *extent = img->GetExtent();

        for(unsigned i = extent[0]; i < extent[1]; i++){
            for(unsigned j = extent[2]; j < extent[3]; j++){
                for(unsigned k = extent[4]; k < extent[5]; k++){

                    unsigned char *imgpoint = ((unsigned char*)img->GetScalarPointer(i, j, k));
                    double temp = (double)(0.299*imgpoint[0] + 0.587*imgpoint[1] + 0.114*imgpoint[2]);
                    unsigned short val = temp*255.0;

                    vector< double > rgb;
                    rgb.push_back(0.299*imgpoint[0]);
                    rgb.push_back(0.587*imgpoint[1]);
                    rgb.push_back(0.114*imgpoint[2]);

                    colormap[val] = rgb;
                }
            }
        }



        map< unsigned short, vector< double > >::iterator it;
        for(it = colormap.begin(); it != colormap.end(); ++it){

            colorf->AddRGBPoint((*it).first, (*it).second[0] / 255.0, (*it).second[1] / 255.0, (*it).second[2] / 255.0);
        }

        prop3Dvect.push_back(data);

        data->setId(_idCount);
        EnableBoundingBox(interactor, data->getId());
        DisableBoundingBox(data->getId());
        _idCount++;*/


    }else{
        image = img;

        VolumeRendererManagerData* data = new VolumeRendererManagerData(img, "");
        prop3Dvect.push_back(data);


        data->setId(_idCount);
        _idCount++;

        EnableBoundingBox(interactor, data->getId());
        DisableBoundingBox(data->getId());



        return data->getId();

    }
}

/**
  *  @pre the image is not null and has more than one scalar component
  *  @post Each component in the image is put in a single image
  *  @param vtkImageData* img, multiple component image i.e. an image of vectors like an rgb
  *  @return vtkImageData* double type image
  */
void VolumeRendererManager::GetImageDouble(vtkImageData* img, vtkImageData* imgushort){


    int *extent = img->GetExtent();

    for(unsigned i = extent[0]; i < extent[1]; i++){
        for(unsigned j = extent[2]; j < extent[3]; j++){
            for(unsigned k = extent[4]; k < extent[5]; k++){
                if(img->GetScalarType() == VTK_UNSIGNED_CHAR){
                    unsigned char *imgpoint = ((unsigned char*)img->GetScalarPointer(i, j, k));

                    unsigned short *vectimgpoint = (unsigned short*)imgushort->GetScalarPointer(i, j, k);
                    double temp = (double)(0.299*imgpoint[0] + 0.587*imgpoint[1] + 0.114*imgpoint[2]);
                    *vectimgpoint = temp*255.0;

                }
            }
        }
    }
}

/**
  *  @pre the image is not null and has more than one scalar component
  *  @post Each component in the image is separated to form a different image
  *  @param vtkImageData* img, multiple component image i.e. an image of vectors like an rgb
  *  @return vector<vtkImageData* > a vector of images, one for each component
  */
void VolumeRendererManager::GetImages(vtkImageData* img, vector<vtkImageData* >& vectimg){

    for(unsigned i = 0; i < img->GetNumberOfScalarComponents(); i++){
        vectimg.push_back(vtkImageData::New());
        vectimg[i]->SetExtent(img->GetExtent());
        vectimg[i]->SetSpacing(img->GetSpacing());
        vectimg[i]->SetOrigin(img->GetOrigin());
        vectimg[i]->AllocateScalars(img->GetScalarType(), 1);
    }

    int *extent = img->GetExtent();

    for(unsigned i = extent[0]; i < extent[1]; i++){
        for(unsigned j = extent[2]; j < extent[3]; j++){
            for(unsigned k = extent[4]; k < extent[5]; k++){
                if(img->GetScalarType() == VTK_UNSIGNED_CHAR){
                    unsigned char *imgpoint = ((unsigned char*)img->GetScalarPointer(i, j, k));

                    for(unsigned l = 0; l < vectimg.size(); l++){
                        unsigned char *vectimgpoint = (unsigned char*)vectimg[l]->GetScalarPointer(i, j, k);
                        *vectimgpoint = imgpoint[l];
                    }
                }else if(img->GetScalarType() == VTK_CHAR){
                    char *imgpoint = ((char*)img->GetScalarPointer(i, j, k));

                   for(unsigned l = 0; l < vectimg.size(); l++){
                        char *vectimgpoint = ( char*)vectimg[l]->GetScalarPointer(i, j, k);
                       *vectimgpoint = imgpoint[l];
                   }
               }else if(img->GetScalarType() == VTK_UNSIGNED_SHORT){
                    unsigned short *imgpoint = ((unsigned short*)img->GetScalarPointer(i, j, k));

                    for(unsigned l = 0; l < vectimg.size(); l++){
                        unsigned short *vectimgpoint = (unsigned short*)vectimg[l]->GetScalarPointer(i, j, k);
                        *vectimgpoint = imgpoint[l];
                    }
                }else if(img->GetScalarType() == VTK_SHORT){
                     short *imgpoint = ((short*)img->GetScalarPointer(i, j, k));

                    for(unsigned l = 0; l < vectimg.size(); l++){
                         short *vectimgpoint = ( short*)vectimg[l]->GetScalarPointer(i, j, k);
                        *vectimgpoint = imgpoint[l];
                    }
                }
            }
        }
    }
}

/**
**	Adds a prop3D to the manager and returns the identifier
**/
int VolumeRendererManager::addVolume(int idTP, vtkImageData* vol, std::string dataname) throw(char*){
        checkInvariant();
        image = vol;
        if(vol != NULL){
                VolumeRendererManagerData* data = new VolumeRendererManagerData(vol, dataname);
                prop3Dvect.push_back(data);
                _renderer->AddActor(data->getProp3D());
                if(idTP == -1)
                {
                        data->setId(_idCount);
                        _idCount++;
                }
                else
                {
                        data->setId(idTP);
                }
                printf("VolumeRendererManager::addVolume->idVolumeRenderer: %i\n", data->getId());
                return data->getId();
        }else{
                throw "Check mhd imagefile file or input";
        }
        return -1;
}
/**
**	adds or removes an actor depending of the bool value
**/

void VolumeRendererManager::addRemoveActor(int propid, bool addremove)  throw(char*){
        checkInvariant();

        VolumeRendererManagerData* data = this->getViewData(propid);
        if(data->getProp3D()!=NULL){
                if(addremove){
                        _renderer->AddViewProp(data->getProp3D());
                }else{
                        _renderer->RemoveViewProp(data->getProp3D());
                }
                _renderer->Render();
        }

}
/**
**	Changes the opacity in a prop3D
**/
void VolumeRendererManager::setVolumeOpacity(int propid, std::vector<double> greylevel,std::vector<double> value)  throw(char*){
	checkInvariant();

	printf("EED VolumeRendererManager::setVolumeOpacity %d\n",propid);
	
	VolumeRendererManagerData* volrenman = this->getViewData(propid);
	if (volrenman!=NULL) 
	{
        volrenman->setVolumeOpacity(greylevel, value);
	} else {
		printf("EED  VolumeRendererManager::setVolumeOpacity  Warning volrenman NULL\n");
	}
	
        _renderer->Render();

}

/**
**	Set Volume Color
**/

void VolumeRendererManager::setVolumeColor(int volid, std::vector<double> greylevel,
                                                                std::vector<double> red,
                                                                std::vector<double> green,
                                                                std::vector<double> blue)throw(char*){
	printf("EED VolumeRendererManager::setVolumeColor  start \n");
	checkInvariant();

	printf("EED VolumeRendererManager::setVolumeColor %d\n",volid);
	
	VolumeRendererManagerData* volrenman = this->getViewData(volid);
	if (volrenman!=NULL) 
	{
		this->getViewData(volid)->setVolumeColor(greylevel, red, green, blue);
	} else {
		printf("EED  VolumeRendererManager::setVolumeColor  Warning volrenman NULL\n");
	}

	_renderer->Render();
	printf("EED VolumeRendererManager::setVolumeColor  end \n");
}

vtkImageData* VolumeRendererManager::getImageData(std::string filename){
        if(filename.compare("")!= 0){

                vtkMetaImageReader* reader =  vtkMetaImageReader::New();
                reader->SetFileName(filename.c_str());
                reader->Update();
                vtkImageData* img = reader->GetOutput();

                vtkImageCast* cast = vtkImageCast::New();
                cast->SetInputData(img);
                cast->SetOutputScalarTypeToUnsignedShort();
                cast->Update();
                //reader->Delete();
                //img->Delete();
                return cast->GetOutput();
                //return img;
        }
        return NULL;
}

vtkImageData* VolumeRendererManager::getImageData(){
        return image;
}

void VolumeRendererManager::checkInvariant()  throw(char*){
	printf("EED VolumeRendererManager::checkInvariant start\n");
	if(this->_renderer==NULL){
                throw "Renderer not set";
        }
	printf("EED VolumeRendererManager::checkInvariant end \n");
}

VolumeRendererManagerData* VolumeRendererManager::getViewData(int id) throw(char*){
    int i;
        for(i = 0; i < (int)(prop3Dvect.size());i++){
                if(prop3Dvect[i]->getId() == id){
                        return prop3Dvect[i];
                }
        }
        throw "id not found in the data";

        return NULL;
}

void VolumeRendererManager::deleteActor(int propid) throw (char *){
        checkInvariant();

        this->addRemoveActor(propid, false);

        int i,n;
        bool exit = false;
        for(i = 0; i < (int)(prop3Dvect.size())&&!exit;i++){
                if(prop3Dvect[i]->getId() == propid){
                        n=i;
                        exit = true;
                }
        }
        if(exit){
                VolumeRendererManagerData* data = prop3Dvect[n];
                int j;
                for(j = i; j < (int)(prop3Dvect.size())-1;j++){
                        prop3Dvect[j] = prop3Dvect[j+1];
                }
                delete data;
                prop3Dvect.pop_back();
        }else{
                throw "id not found in the data";
        }

}

vtkPiecewiseFunction* VolumeRendererManager::GetTransferFunction(int volumeid){
        return getViewData(volumeid)->GetTransferFunction();
}
vtkColorTransferFunction* VolumeRendererManager::GetColorFunction(int volumeid){

        return getViewData(volumeid)->GetColorFunction();
}

void VolumeRendererManager::changeCompositeMIPFunction(int id, int function) throw (char *){
    if(id == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->changeCompositeMIPFunction(function);
    }else{
        getViewData(id)->changeCompositeMIPFunction(function);
    }
}

/**
  Changes the interpolation of the volume rendering.
  type == 0 for linear interpolation
  type == 1 for nearest interpolation
  */
void VolumeRendererManager::changeInterpolationType(int type, int propid){
    if(propid == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->changeInterpolationType(type);
    }else{
        getViewData(propid)->changeInterpolationType(type);
    }
}

/**
  * Set the lookuptable to the volumes in memory
  * if the id is set then it only changes the lookup table for a specific volume
 */
void VolumeRendererManager::SetLookupTable(vtkLookupTable* lookup, int id){
    if(id == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->SetLookupTable(lookup);
    }else{
        getViewData(id)->SetLookupTable(lookup);
    }

}

/**
  * @returns all the props3D in this manager
*/
vector< vtkProp3D* > VolumeRendererManager::getProps3D(){

    vector< vtkProp3D* >  propvects;
    for(unsigned i = 0; i < prop3Dvect.size(); i++){
        propvects.push_back(prop3Dvect[i]->getProp3D());
    }
    return propvects;
}

/**
  *  @param std::vector<double> greylevel, the corresponding greylevel in the image
  *  @param std::vector<double> value, the corresponding value for the opacity
  *  @param int propid, the correspoding id, by default it applies the changes to the first volume in the array
  */
void VolumeRendererManager::setVolumeOpacity(std::vector<double> greylevel, std::vector<double> value, int propid){
    if(propid == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->setVolumeOpacity(greylevel, value);
    }else{
        getViewData(propid)->setVolumeOpacity(greylevel, value);
    }
}

void VolumeRendererManager::EnableBoundingBox(vtkRenderWindowInteractor* interactor, int propid){
    if(propid == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->EnableBoundingBox(interactor);
    }else{
        getViewData(propid)->EnableBoundingBox(interactor);
    }
}

void VolumeRendererManager::DisableBoundingBox(int propid){

    if(propid == -1){
        for(unsigned i = 0; i < prop3Dvect.size(); i++)
            prop3Dvect[i]->DisableBoundingBox();
    }else{
        getViewData(propid)->DisableBoundingBox();
    }
}
