#ifndef WXMARACASSURFACERENDERINGMANAGERDATAMHD_H_
#define WXMARACASSURFACERENDERINGMANAGERDATAMHD_H_


#include "wxMaracasSurfaceRenderingManagerData.h"
#include "boxSurfaceObserver.h"

#include "vtkMetaImageReader.h"
#include "vtkMarchingCubes.h"
#include "vtkCleanPolyData.h"
#include "vtkImageData.h"
#include "vtkClipPolyData.h"
#include "vtkPlanes.h"
#include "vtkImageThreshold.h"
#include "vtkSmoothPolyDataFilter.h"

class wxMaracasSurfaceRenderingManagerDataMhd : public wxMaracasSurfaceRenderingManagerData  {

public:
	
	wxMaracasSurfaceRenderingManagerDataMhd(vtkImageData* imagedata, std::string dataname="", vtkRenderWindowInteractor*  interactor=0);
	~wxMaracasSurfaceRenderingManagerDataMhd();

	
	/**
	**	Adds a prop3D to the world of the application
	**/
	void setVtkImageData(vtkImageData* imagedata);
	
	/**
	**	changes the isovalue in a prop3D
	**/
	void changeIsoValue(double value);	
        /**
        **	changes the isovalue in a prop3D
        **/
        void changeIsoValue(double min, double max);
	/**
	**	Check if the variables are setted correctly
	**/
	void checkInvariant();	
	
	/**
	**	Returns the grey max level of the image
	**/

	int getMaxGreyLevel();

	/**
	**	creates the image 
	**/
	void contourExtractor(int isovalue);		
	
	/**
	** Updates surface
	**/
	void UpdateSurface();

        virtual void enableBoxWidget(bool enable);
private:	
	
	vtkImageData* _imagedata;
	int _maxgreylevel;

	/**
	** to the image render
	**/
	vtkMarchingCubes* _cubesFilter;
        vtkCleanPolyData* _cleanFilter;
	vtkClipPolyData* _tissueClipper;
	vtkPlanes* _tissuePlanes;
        vtkImageThreshold * _tresholdFilter;
        vtkSmoothPolyDataFilter* _smooth;
	
	/**
	** Get's the max grey level of the image
	**/
	int getMaxLevel(vtkImageData* img);
	

	
};

#endif /*WXMARACASSURFACERENDERINGMANAGERDATAMHD_H_*/
