
/*=========================================================================

  Program:   wxMaracas
  Module:    $RCSfile: volumerenderermanager.h,v $
  Language:  C++
  Date:      $Date: 2011/10/05 16:27:04 $
  Version:   $Revision: 1.2 $

  Copyright: (c) 2002, 2003
  License:

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/




#ifndef __VolumeRendererManagerH__
#define __VolumeRendererManagerH__

#include <iostream>
#include <vector>
#include <vtkMatrix4x4.h>

#include "volumerenderermanagerdata.h"


class VolumeRendererManager  {

public:
        VolumeRendererManager();
        ~VolumeRendererManager();

        /**
        **	Sets the renderer to manage the prop3D from the view
        **/
        void setRenderer(vtkRenderer*  renderer);

        /**
        **	Gets the renderer to manage the prop3D from the view
        **/
        vtkRenderer* getRenderer();

        /**
        **	Adds a volume
        **/
        int addVolume(int idTP, vtkImageData* img, std::string dataname) throw (char*);

        /**
        * @pre   The image can have one or multiple components per voxel, and volume rendering is performed seprately over the
                 three of them. If the image has multiple components and the separate components flag is set to false, then
                 the vtkImageAppendComponents is used to create a single image.
        * @post  The volume rendering is performed over the image vol
        * @param vtkImageData* the image volume
        * @param bool independentcomponents, if the image has multiple components, the mapper will be created either for managing
                        an rgb image plus a luminance channel (the luminance will be calculated using the luminance image filter)
                        to control the opacity or to manage the multiple components in a single image
        */
        int addVolume(vtkImageData* img, vtkRenderWindowInteractor* interactor, bool independentcomponents = false);

        /**
        **	loads a prop3D from a nSTL file
        **/
        vtkProp3D* getVolume(std::string filename);

        /**
        **	loads a MHD file to convert it into an actor
        **/
        vtkImageData* getImageData(std::string filename);

        /**
        ** Gets image data asotiated with the rendering manager
        **/
        vtkImageData* getImageData();

        /**
        **	adds or removes an actor depending of the bool value
        **/
        void addRemoveActor(int propid, bool addremove) throw(char*);

        /**
        **	Check if the variables are setted correctly
        **/
        void checkInvariant()throw(char*);

        /**
        ** Set Volume Opacity
        **/
        void setVolumeOpacity(int propid, std::vector<double> greylevel,std::vector<double> value) throw(char*);
        /**
        **	Set Volume Color
        **/
        void setVolumeColor(int volid, std::vector<double> greylevel,
                                                                        std::vector<double> red,
                                                                        std::vector<double> green,
                                                                        std::vector<double> blue)throw(char*);

        /**
        ** Given an id search the data in the vector
        **/
        VolumeRendererManagerData* getViewData(int id)throw(char*);

        void changeCompositeMIPFunction(int id, int function) throw (char *);

        /**
        ** Deletes given actor
        **/
        void deleteActor(int volumeid)throw (char *);

        /**
        ** Updates given volume
        **/
        void Update(int propid);

        vtkPiecewiseFunction* GetTransferFunction(int volumeid);
        vtkColorTransferFunction* GetColorFunction(int volumeid);


        /**
          Changes the interpolation of the volume rendering.
          type == 0 for linear interpolation
          type == 1 for nearest interpolation
          */
        void changeInterpolationType(int type, int propid = -1);

        /**
          * Set the lookuptable to the volumes in memory
          * if the id is set then it only changes the lookup table for a specific volume
         */
        void SetLookupTable(vtkLookupTable* lookup, int id = -1);

        /**
          * @returns all the props3D in this manager
        */
        vector< vtkProp3D* > getProps3D();

        /**
          *  @param std::vector<double> greylevel, the corresponding greylevel in the image
          *  @param std::vector<double> value, the corresponding value for the opacity
          *  @param int propid, the correspoding id, by default it applies the changes to the first volume in the array
          */
        void setVolumeOpacity(std::vector<double> greylevel, std::vector<double> value, int propid = -1);


        void EnableBoundingBox(vtkRenderWindowInteractor* interactor, int propid = -1);

        void DisableBoundingBox(int propid = -1);
private:
        std::vector<VolumeRendererManagerData*> prop3Dvect;

        vtkRenderer*  _renderer;
        vtkImageData* image;

        int _idCount;

        /**
          *  @pre the image is not null and has more than one scalar component
          *  @post Each component in the image is separated to form a different image
          *  @param vtkImageData* img, multiple component image i.e. an image of vectors like an rgb
          *  @return vector<vtkImageData* > a vector of images, one for each component
          */
        void GetImages(vtkImageData* img, vector<vtkImageData* >& images);

        /**
          *  @pre the image is not null and has more than one scalar component
          *  @post Each component in the image is put in a single image
          *  @param vtkImageData* img, multiple component image i.e. an image of vectors like an rgb
          *  @return vtkImageData* double type image
          */
        void GetImageDouble(vtkImageData* img, vtkImageData* imgdouble);



};

#endif
