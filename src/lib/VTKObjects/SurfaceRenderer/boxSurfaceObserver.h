#ifndef BOXSURFACEOBSERVER_H_
#define BOXSURFACEOBSERVER_H_

#include "vtkPlanes.h"
#include "vtkProp.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkObject.h"
#include "vtkCommand.h"


// Callback for the interaction
class boxSurfaceObserver : public vtkCommand
{
	public:
		vtkPlanes					*_planes;
		vtkProp						*_actor;
		vtkVolumeRayCastMapper		*_vtkVolumeRayCastMapper;

		boxSurfaceObserver() 
		{  
			_vtkVolumeRayCastMapper = NULL;
		}

		virtual char const *GetClassName() const { return "boxSurfaceObserver";}

		static boxSurfaceObserver *New(){
			boxSurfaceObserver * result;
			result = new boxSurfaceObserver();
			return result;
		}

		virtual void Execute(vtkObject *wdg, unsigned long eventId, void* calldata) ;
		void SetPlanes(vtkPlanes *planes);
		void SetActor(vtkProp *actor);
		void SetvtkVolumeRayCastMapper(vtkVolumeRayCastMapper *vtkvolumeraycastmapper);
};



#endif /*BOXSURFACEOBSERVER_H_*/
