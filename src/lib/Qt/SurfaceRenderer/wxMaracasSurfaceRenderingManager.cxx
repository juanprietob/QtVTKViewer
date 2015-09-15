/*=========================================================================

  Program:   wxMaracas
  Module:    $RCSfile: wxMaracasSurfaceRenderingManager.cxx,v $
  Language:  C++
  Date:      $Date: 2011/10/05 16:26:55 $
  Version:   $Revision: 1.2 $

  Copyright: (c) 2002, 2003
  License:

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "wxMaracasSurfaceRenderingManager.h"
#include "wxMaracasSurfaceRenderingManagerDataMhd.h"


#include "vtkPLYReader.h"
#include "vtkSmartPointer.h"

/**
**	Start of the manager class
**/
wxMaracasSurfaceRenderingManager::wxMaracasSurfaceRenderingManager(){
	_renderer = NULL;
	_interactor = NULL;
	_idCount=0;
}
wxMaracasSurfaceRenderingManager::~wxMaracasSurfaceRenderingManager(){
}

/**
**	Sets the renderer to manage the prop3D from the surface render
**/
void wxMaracasSurfaceRenderingManager::setRenderer(vtkRenderer*  renderer){
	_renderer = renderer;
}

/**
**	Sets the renderer to manage the prop3D from the surface render
**/
void wxMaracasSurfaceRenderingManager::setInteractor(vtkRenderWindowInteractor*  interactor){
	_interactor = interactor;
}


/**
** Gets the renderer to manage the prop3D from the surface render
**/
vtkRenderer* wxMaracasSurfaceRenderingManager::getRenderer(){
	return _renderer;
}

/**
** Updates volume
**/
void wxMaracasSurfaceRenderingManager::Update(int pid)throw(char*){
	wxMaracasSurfaceRenderingManagerData* data = this->getViewData(pid);	
	((wxMaracasSurfaceRenderingManagerDataMhd*)data)->UpdateSurface();
	_renderer->Render();
}

/**
**	Adds a prop3D to the manager and returns the identifier
**/
int wxMaracasSurfaceRenderingManager::addProp3D(int idTP, vtkProp3D* prop3D, std::string dataname)  throw(char*){
        //checkInvariant();
	if(prop3D != NULL){
		wxMaracasSurfaceRenderingManagerData* data = new wxMaracasSurfaceRenderingManagerData(prop3D, dataname, _interactor);
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
		return data->getId();
	}else{
		throw "Check vtkProp3D file or input";
	}
	return -1;
}
int wxMaracasSurfaceRenderingManager::addPropMHD(int idTP, vtkImageData* imagedata, std::string dataname) throw(char*){
	checkInvariant();
	if(imagedata != NULL){                
		image = imagedata;
		wxMaracasSurfaceRenderingManagerData* data = new wxMaracasSurfaceRenderingManagerDataMhd(imagedata, dataname, _interactor);
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
		printf("wxMaracasSurfaceRenderingManager::addPropMHD->idSurfaceRender: %i\n", data->getId());
		return data->getId();
	}else{
		throw "Check ImageData file or input";
	}
	return -1;
}
/**
**	adds or removes an actor depending of the bool value
**/
void wxMaracasSurfaceRenderingManager::addRemoveActor(int propid, bool addremove)  throw(char*){
	checkInvariant();
	
        wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);
        if(data && data->getProp3D()!=NULL){
            if(addremove){
                _renderer->AddViewProp(data->getProp3D());
            }else{
                _renderer->RemoveViewProp(data->getProp3D());
            }
            _renderer->Render();
        }else if(addremove && propid == 0 && image){
            addPropMHD(0, image, "id0");
        }
}
/**
**	adds or removes the surface box depending of the bool value
**/
void wxMaracasSurfaceRenderingManager::addRemoveSurfaceBox(int propid, bool addremove)  throw(char*){
	checkInvariant();
	
	wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);		
	if(data->getProp3D()!=NULL){
		data->addRemoveSurfaceBox(addremove);
		/*if(addremove){
			data->
			_renderer->AddViewProp(data->getProp3D());
		}else{
			_renderer->RemoveViewProp(data->getProp3D());
		}
		_renderer->Render();*/
	}
	
}

/**
**	Changes the opacity in a prop3D
**/
void wxMaracasSurfaceRenderingManager::changeOpacity(int propid, int value)  throw(char*){
	checkInvariant();	


	this->getViewData(propid)->changeOpacity(value);
	
	_renderer->Render();

}

/**
**	changes the isovalue in a prop3D
**/
void wxMaracasSurfaceRenderingManager::changeIsoValue(int propid, double value ){
	checkInvariant();	

	wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);		

	//_renderer->RemoveActor(data->getProp3D());
	((wxMaracasSurfaceRenderingManagerDataMhd*)data)->changeIsoValue(value);
	//_renderer->AddActor(data->getProp3D());

	_renderer->Render();
}

/**
**	changes the isovalue in a prop3D
**/
void wxMaracasSurfaceRenderingManager::changeIsoValue(int propid, double min, double max ){
        checkInvariant();

        wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);

        //_renderer->RemoveActor(data->getProp3D());
        ((wxMaracasSurfaceRenderingManagerDataMhd*)data)->changeIsoValue(min, max);
        //_renderer->AddActor(data->getProp3D());

        _renderer->Render();
}

vtkProp3D* wxMaracasSurfaceRenderingManager:: getProp3D(std::string filename){
	if(filename.compare("")!= 0){

            vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
            std::string ext = filename.substr(filename.find_last_of("."), 4);
            if(ext.compare(STL)==0){
                vtkSmartPointer<vtkSTLReader> STLReader=vtkSmartPointer<vtkSTLReader>::New();
		STLReader->SetFileName(filename.c_str());
		STLReader->Update();
                polydata->DeepCopy(STLReader->GetOutput());

            }else if(ext.compare(PLY)==0){
                vtkSmartPointer<vtkPLYReader> plyreader =vtkSmartPointer<vtkPLYReader>::New();
                plyreader->SetFileName(filename.c_str());
                plyreader->Update();
                polydata->DeepCopy(plyreader->GetOutput());
            }

            vtkSmartPointer<vtkPolyDataMapper> dataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            dataMapper->SetInputData(polydata);

            vtkActor* dataActor = vtkActor::New();
            dataActor->SetMapper(dataMapper);
            dataActor->GetProperty()->SetOpacity(1);
            return dataActor;
	}	
	return NULL;
}

vtkImageData* wxMaracasSurfaceRenderingManager::getImageData(std::string filename){
	if(filename.compare("")!= 0){	
		
		vtkMetaImageReader* reader =  vtkMetaImageReader::New();	
		reader->SetFileName(filename.c_str());
		reader->Update();
		vtkImageData* img = reader->GetOutput();
		//reader->Delete();
		return img;
	}	
	return NULL;
}

vtkImageData* wxMaracasSurfaceRenderingManager::getImageData(){
	return image;
}

void wxMaracasSurfaceRenderingManager::setImageData(vtkImageData* img){
    image = img;
}

void wxMaracasSurfaceRenderingManager::checkInvariant()  throw(char*){
	if(this->_renderer==NULL){
		throw "Renderer not set";
	}
}

wxMaracasSurfaceRenderingManagerData* wxMaracasSurfaceRenderingManager::getViewData(int id) throw(char*){
    int i;
    for(i = 0; i < (int)(prop3Dvect.size());i++){
            if(prop3Dvect[i]->getId() == id){
                    return prop3Dvect[i];
            }
    }
    return 0;
#ifndef USE_QT
    throw "id not found in the data";
#else
    return NULL;
#endif


}

int wxMaracasSurfaceRenderingManager::getMaxIsoValue(int propid) throw(char*){
	
	return ((wxMaracasSurfaceRenderingManagerDataMhd*)this->getViewData(propid))->getMaxGreyLevel();

}

void wxMaracasSurfaceRenderingManager::changeColor(int propid, double red, double green, double blue) throw(char*){
	checkInvariant();	
	this->getViewData(propid)->changeColor(red, green, blue);

	_renderer->Render();
}

void wxMaracasSurfaceRenderingManager::deleteActor(int propid) throw (char *){
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
		wxMaracasSurfaceRenderingManagerData* data = prop3Dvect[n];			
		int j;
		for(j = i; j < (int)(prop3Dvect.size())-1;j++){
			prop3Dvect[j] = prop3Dvect[j+1];
		}		
		delete data;
		prop3Dvect.pop_back();
    }
}

void wxMaracasSurfaceRenderingManager::enableBoundingBox(int propid, bool enable){
    checkInvariant();

    wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);

    if(data)
        data->enableBoxWidget(enable);
}
void wxMaracasSurfaceRenderingManager::Transform(vtkMatrix4x4* tmatrix){
	
}

bool wxMaracasSurfaceRenderingManager::interactorSet(){
	return _interactor? true:false;
}

void wxMaracasSurfaceRenderingManager::saveProp3DSTL(int propid,const char* filename){
    checkInvariant();

    wxMaracasSurfaceRenderingManagerData* data = this->getViewData(propid);
    if(data->getProp3D()!=NULL){
            data->saveProp3DSTL(filename);
    }
}

void wxMaracasSurfaceRenderingManager::loadProp3DSTL(const char* filename){
    this->addProp3D(-1, this->getProp3D(filename), filename);
}
