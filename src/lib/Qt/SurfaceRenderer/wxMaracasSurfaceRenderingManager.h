/*=========================================================================

  Program:   wxMaracas
  Module:    $RCSfile: wxMaracasSurfaceRenderingManager.h,v $
  Language:  C++
  Date:      $Date: 2011/10/05 16:26:55 $
  Version:   $Revision: 1.2 $

  Copyright: (c) 2002, 2003
  License:
  
     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/




#ifndef __wxMaracasSurfaceRenderingManagerH__
#define __wxMaracasSurfaceRenderingManagerH__

#include <iostream>
#include <vector>
#include <vtkMatrix4x4.h>

#include "wxMaracasSurfaceRenderingManagerData.h"

class wxMaracasSurfaceRenderingManager  {

public:
	wxMaracasSurfaceRenderingManager();
	~wxMaracasSurfaceRenderingManager();

	/**
	**	Sets the renderer to manage the prop3D from the surface render
	**/
	void setRenderer(vtkRenderer*  renderer);

	/**
	**	Gets the renderer which manages the prop3D from the surface render
	**/
	vtkRenderer* getRenderer();
	
	/**
	** Updates surface
	**/
	void Update(int pid)throw(char*);
	
	/**
	**	Adds a prop3D to the manager and returns the identifier
	**/
	int addProp3D(int idTP, vtkProp3D* prop3D, std::string dataname) throw (char*);
	/**
	**	Adds a prop3D to the manager and returns the identifier
	**/
	int addPropMHD(int idTP, vtkImageData* imagedata, std::string dataname) throw(char*);
	/**
	**	Changes the opacity in a prop3D
	**/
	void changeOpacity(int propid, int value)throw(char*);

	/**
	**	changes the isovalue in a prop3D
	**/
        void changeIsoValue(int propid, double value);

        void changeIsoValue(int propid, double min, double max );

	/**
	**	loads a prop3D from a nSTL file
	**/
	vtkProp3D* getProp3D(std::string filename);

	/**
	**	loads a MHD file to convert it into an actor
	**/
	vtkImageData* getImageData(std::string filename);

	/**
	** Gets image data asotiated with the rendering manager
	**/
	vtkImageData* getImageData();

        virtual void setImageData(vtkImageData* img);

	/**
	**	adds or removes an actor depending of the bool value
	**/
	void addRemoveActor(int propid, bool addremove)throw(char*);
	/**
	**
	**/
	void addRemoveSurfaceBox(int propid, bool addremove)  throw(char*);
	/**
	**	Check if the variables are setted correctly
	**/

	void checkInvariant()throw(char*);

	/**
	**	Given an id search the data in the vector
	**/
	wxMaracasSurfaceRenderingManagerData* getViewData(int id)throw(char*);

	/**
	**
	**/
	void setInteractor(vtkRenderWindowInteractor*  interactor);

	/**
	**	Given the id, return the max iso value from the imagedata
	**/
	int getMaxIsoValue(int propid)throw(char*);


	/**
	**	Changes the color of the actor
	**/
	void changeColor(int propid, double red, double green, double blue) throw(char*);

	void deleteActor(int propid)throw (char *);

	void Transform(vtkMatrix4x4* tmatrix);

	bool interactorSet();


        void enableBoundingBox(int propid, bool enable);

        void saveProp3DSTL(int propid,const char* filename);

        void loadProp3DSTL(const char* filename);


	
private:	
	std::vector<wxMaracasSurfaceRenderingManagerData*> prop3Dvect;

	vtkRenderer*  _renderer;
	vtkRenderWindowInteractor*  _interactor;
	vtkImageData* image;

	int _idCount;

	

	
};

#endif 
