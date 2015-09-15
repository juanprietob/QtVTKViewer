#ifndef WXMARACASSURFACERENDERINGMANAGERDATA_H_
#define WXMARACASSURFACERENDERINGMANAGERDATA_H_

#include "vtkProp3D.h"
#include "vtkRenderer.h"
#include "vtkSTLReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkBoxWidget.h"


#define PLY  ".ply"
#define STL  ".stl"


class wxMaracasSurfaceRenderingManagerData {

public:
	wxMaracasSurfaceRenderingManagerData(vtkProp3D* _prop3Dvect, std::string dataname="", vtkRenderWindowInteractor*  interactor=NULL);	
	~wxMaracasSurfaceRenderingManagerData();	
	
	/**
	**	Adds a prop3D to the world of the application
	**/
	void setProp3D(vtkProp3D* prop3D);
	/**
	**	Changes the opacity in a prop3D
	**/
	void changeOpacity(int value);		
	/**
	**	Check if the variables are setted correctly
	**/
	void checkInvariant();
	/**
	**	get the prop3D 
	**/
	vtkProp3D* getProp3D();
	/**
	**	return the id from the daat
	**/
	int getId();
	/**
	**	set data id
	**/
	void setId(int propid);
	/**
	**	Get the filanme
	**/
    std::string getDataname();
	/**
	** Set the filanme
	**/
    void setDataname(std::string dataname);
	
	/**
	**	creates the image 
	**/
	void contourExtractor(int isovalue);

	/**
	**	Changes the color of the actor
	**/
	void changeColor(double red, double green, double blue);	
	
	/**
	** adds or removes the surface box
	**/
	void addRemoveSurfaceBox(bool visible);

	void initializeBoxWidget(vtkRenderWindowInteractor*  interactor);

        void saveProp3DSTL(const char* filename);

        virtual void enableBoxWidget(bool enable);

protected:
        vtkPolyDataMapper* _dataMapper;
	/**
	 * Prop 3D (data actor)
	 */
	vtkProp3D* _prop3D;	
	/**
	 *  Dataname given by the user (ex. filename) 
	 **/
	std::string _dataname;	
	
	/**
	**
	**/
	vtkBoxWidget* _boxWidgetS1;
private:
	
	/*
	 * id of the data
	 */
	int _id;		

	

	
};

#endif /*WXMARACASSURFACERENDERINGMANAGERDATA_H_*/
