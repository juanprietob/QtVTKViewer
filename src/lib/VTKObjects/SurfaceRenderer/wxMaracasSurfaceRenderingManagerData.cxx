
#include "wxMaracasSurfaceRenderingManagerData.h"

#include <vtkSmartPointer.h>
#include "vtkSTLWriter.h"
#include "vtkPLYWriter.h"

/********************************************************************************************
** Start of data viewmanagerData
*********************************************************************************************/

wxMaracasSurfaceRenderingManagerData::wxMaracasSurfaceRenderingManagerData(vtkProp3D* prop3Dvect, std::string dataname, vtkRenderWindowInteractor*  interactor){

        _dataMapper = 0;
	_prop3D = prop3Dvect;	
	_dataname = dataname;	
	_boxWidgetS1=NULL;
	
	initializeBoxWidget(interactor);
	

	

	/*_boxWidgetS1->GetPlanes( this->GetVtkClipping3DDataViewer()->GetTissuePlanes(0) );
	_boxWidgetS1->GetPlanes( this->GetVtkClipping3DDataViewer()->GetTissuePlanes(1) );
	_boxWidgetS1->GetPlanes( this->GetVtkClipping3DDataViewer()->GetTissuePlanes(2) );
	_boxWidgetS1->GetPlanes( this->GetVtkClipping3DDataViewer()->GetTissuePlanes(3) );*/
}


wxMaracasSurfaceRenderingManagerData::~wxMaracasSurfaceRenderingManagerData(){
	
	_prop3D->Delete();	
	if (_boxWidgetS1!=NULL)					 { _boxWidgetS1		-> Delete();					}
}

void wxMaracasSurfaceRenderingManagerData::initializeBoxWidget(vtkRenderWindowInteractor*  interactor){
	if(interactor!= NULL){
	
	}
}

/**
**	Adds a prop3D to the world of the application
**/
void wxMaracasSurfaceRenderingManagerData::setProp3D(vtkProp3D* prop3D){
	_prop3D = prop3D;
}

void wxMaracasSurfaceRenderingManagerData::addRemoveSurfaceBox(bool visible)
{
	if(_boxWidgetS1){
		if (visible==true){
			_boxWidgetS1->On();
		} else {
			_boxWidgetS1->Off();
		}
	}
}
/**
**	Changes the opacity in a prop3D
**/
void wxMaracasSurfaceRenderingManagerData::changeOpacity(int value){
	std::cout<<"chage op"<<value<<std::endl;
	vtkActor* actor = (vtkActor*)this->_prop3D;	
	actor->GetProperty()->SetOpacity((double)value/100.0);

	

}
void wxMaracasSurfaceRenderingManagerData::changeColor(double red, double green, double blue){
	std::cout<<"chage col"<<red<<green<<blue<<std::endl;
    vtkActor* actor = (vtkActor*)this->_prop3D;	
	actor->GetProperty()->SetColor(red,green,blue);	
}
/**
**	Check if the variables are setted correctly
**/
void wxMaracasSurfaceRenderingManagerData::checkInvariant(){

}
/**
**	get the prop3D 
**/
vtkProp3D* wxMaracasSurfaceRenderingManagerData::getProp3D(){
	return this->_prop3D;
}
/**
**	return the id from the daat
**/
int wxMaracasSurfaceRenderingManagerData::getId(){
	return _id;
}
/**
**	set data id
**/
void wxMaracasSurfaceRenderingManagerData::setId(int propid){
	_id = propid;
}

/**
**	Get the filanme
**/
std::string wxMaracasSurfaceRenderingManagerData::getDataname(){
	return _dataname;
}
/**
** Set the filanme
**/
void wxMaracasSurfaceRenderingManagerData::setDataname(std::string dataname){
	_dataname = dataname;
}



void wxMaracasSurfaceRenderingManagerData::saveProp3DSTL(const char* filename){
    if(_dataMapper){

        std::string filena(filename);
        std::string ext = filena.substr(filena.find_last_of("."), 4);
        if(ext.compare(PLY) == 0){
            vtkSmartPointer<vtkPLYWriter> plywriter =
            vtkSmartPointer<vtkPLYWriter>::New();
              plywriter->SetFileName(filename);
              plywriter->SetInputData(_dataMapper->GetInput());
              plywriter->Write();
        }else if(ext.compare(PLY) == 0){
            vtkSmartPointer<vtkSTLWriter> stlWriter =
            vtkSmartPointer<vtkSTLWriter>::New();
              stlWriter->SetFileName(filename);
              stlWriter->SetInputData(_dataMapper->GetInput());
              stlWriter->SetFileTypeToBinary();
              stlWriter->Write();
        }else{
            cout<<"unsupported format"<<endl;
        }
    }
}

void wxMaracasSurfaceRenderingManagerData::enableBoxWidget(bool enable){

}




