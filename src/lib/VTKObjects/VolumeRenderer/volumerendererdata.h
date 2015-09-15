#ifndef VOLUMERENDERERDATA_H
#define VOLUMERENDERERDATA_H


#include "volumerenderermanager.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObject.h"

using namespace std;

class VolumeRendererData : public vtkObject
{
public:
    static VolumeRendererData *New();
    vtkTypeRevisionMacro(VolumeRendererData,vtkObject);
    //

    vtkSetObjectMacro(ImageData, vtkImageData);
    //vtkSetMacro(ImageDataSeparateComponents, bool);

    vtkSetObjectMacro(Renderer, vtkRenderer);
    vtkSetObjectMacro(LookUpTable, vtkLookupTable);
    vtkSetObjectMacro(Interactor, vtkRenderWindowInteractor);



    void BoundingBoxChanged(bool checked);
     void ShowVolume(bool enable);
     void OpacityChanged(vector<double> greyvalues, vector<double> values);

     /**
       chages the ray cast function type
       function = 0 composite
       function = 1 MIP (maximum intensity pixel)
       */
    void changeCompositeMIPFunction(int function);

    /**
      changes the interoplation type of the volume
      type = 0 linear
      type = 1 nearest
       */
    void changeInterpolationType(int type);


protected:
    VolumeRendererData();
    VolumeRendererManager* VolRendManager;
    vtkImageData* ImageData;
    vtkRenderer* Renderer;
    vtkLookupTable* LookUpTable;
    vtkRenderWindowInteractor* Interactor;

//    bool ImageDataSeparateComponents;


private:

    void invariant();
};

#endif // VOLUMERENDERERDATA_H
