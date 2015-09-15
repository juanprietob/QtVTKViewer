
#include "boxSurfaceObserver.h"
#include "vtkBoxWidget.h"

void boxSurfaceObserver::Execute(vtkObject *wdg, unsigned long eventId, void* calldata) {  // virtual
	vtkBoxWidget *boxwidget = reinterpret_cast<vtkBoxWidget*>(wdg);
	boxwidget->GetPlanes(_planes);

	if ( _vtkVolumeRayCastMapper != NULL )
	{
		_vtkVolumeRayCastMapper->RemoveAllClippingPlanes();
//		vtkPlanes *planes = vtkPlanes::New();
//		boxwidget->GetPlanes(planes);
//		_vtkVolumeRayCastMapper->SetClippingPlanes(planes);
		_vtkVolumeRayCastMapper->SetClippingPlanes(_planes);
	}

//	_actor->VisibilityOn();

}
//-------------------------------------------------------------------
void boxSurfaceObserver::SetPlanes(vtkPlanes *planes){
	_planes = planes;
}
//-------------------------------------------------------------------
void boxSurfaceObserver::SetActor(vtkProp *actor){
	_actor = actor;
}

//-------------------------------------------------------------------
void boxSurfaceObserver::SetvtkVolumeRayCastMapper(vtkVolumeRayCastMapper *vtkvolumeraycastmapper)
{
	_vtkVolumeRayCastMapper = vtkvolumeraycastmapper;
}
