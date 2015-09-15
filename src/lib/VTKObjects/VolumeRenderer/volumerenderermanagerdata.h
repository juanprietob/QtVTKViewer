
#ifndef VolumeRendererManagerData_H_
#define VolumeRendererManagerData_H_

#include "vtkCommand.h"
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkPlanes.h>
#include <vtkVolumeMapper.h>

#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkProp3D.h>
#include <vtkRenderer.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkLookupTable.h>
#include <vtkBoxWidget.h>
#include <vtkAbstractMapper.h>


#include <vector>

using namespace std;

// Callback for moving the planes from the box widget to the mapper
class vtkBoxWidgetCallback : public vtkCommand
{
public:
  static vtkBoxWidgetCallback *New()
    { return new vtkBoxWidgetCallback; }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
      vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
      vtkPlanes *planes = vtkPlanes::New();
      widget->GetPlanes(planes);
      for(unsigned i = 0; i< VectorMapper.size(); i++){
          VectorMapper[i]->SetClippingPlanes(planes);
      }
      planes->Delete();
    }
  void SetMapper(vtkAbstractMapper* m)
    { if(VectorMapper.size() == 0){
          VectorMapper.push_back(m);
      }else{
          VectorMapper[0] = m;
      }
    }

  void AddMapper(vtkAbstractMapper* m){
      VectorMapper.push_back(m);
  }

protected:
  vtkBoxWidgetCallback()
    {  }

  vector< vtkAbstractMapper* > VectorMapper;
};


class VolumeRendererManagerData  : public vtkObject{

public:
        VolumeRendererManagerData(vtkImageData* vol, std::string dataname="");
        VolumeRendererManagerData(vtkImageData* vol, vtkRenderer* render, std::string dataname="");
        VolumeRendererManagerData(vtkImageData* vol, bool usegpu);


        ~VolumeRendererManagerData();

        /**
        **	Check if the variables are setted correctly
        **/
        void checkInvariant()throw (char *);

        /**
        ** Updates volume
        **/
        void Update();

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
        **	Set Volume Color
        **/
        void setVolumeColor(std::vector<double>& greylevel,
                                                                        std::vector<double>& red,
                                                                        std::vector<double>& green,
                                                                        std::vector<double>& blue);

        /**
        **	Volume Opacity
        **/
        void setVolumeOpacity(std::vector<double> greylevel,std::vector<double> value);
        /**
        **	Volume Opacity
        **/
        void setVolumeOpacity(std::vector<double> greylevel,double value);

        vtkPiecewiseFunction* GetTransferFunction(){
                return _tfun;
        }

        vtkColorTransferFunction* GetColorFunction(){
                return _ctfun;
        }

        void changeCompositeMIPFunction(int function);

        void SetLookupTable(vtkLookupTable* lookuptable);

        void EnableBoundingBox(vtkRenderWindowInteractor* interactor);
        void DisableBoundingBox();

        /**
          changes the interoplation type of the volume
          type = 0 linear
          type = 1 nearest
           */
        void changeInterpolationType(int type);


        vtkGetMacro(BoxWidget, vtkBoxWidget*);
        vtkSetMacro(BoxWidget, vtkBoxWidget*);

        vtkGetMacro(VolumeMapper, vtkVolumeMapper*);
        vtkSetMacro(VolumeMapper, vtkVolumeMapper*);

        void SetColorTransferFunction(int i, vtkColorTransferFunction* colorf);

        void SetIndependentComponents(bool independent);
protected:
        /**
         * Prop 3D (data actor)
         */
        vtkImageData* _vol;
        /**
         *  Dataname given by the user (ex. filename)
         **/
        std::string _dataname;


private:

        /*
         * id of the data
         */
        int _id;

        vtkVolumeRayCastCompositeFunction	*_compositeFunction;
        vtkPlanes				*_volumePlanes;
	
        vtkVolumeProperty					*_volumeProperty;
        vtkVolume							*_newvol;
        vtkPiecewiseFunction* _tfun;
        vtkColorTransferFunction* _ctfun;
        vtkVolumeRayCastMIPFunction* _MIPFunction;
        /**
        ** boxwidget to control the volume
        **/
        vtkBoxWidget* BoxWidget;
        vtkVolumeMapper *VolumeMapper;

        void Initialize(vtkImageData* vol, std::string dataname, bool usegpu = false);


};

#endif /*VolumeRendererManagerData_H_*/
