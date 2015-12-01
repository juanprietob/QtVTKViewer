if( VTK_VERSION_MAJOR EQUAL 6 )
  set(Slicer_VTK_COMPONENTS
    vtkCommonColor
    vtkCommonComputationalGeometry
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonMisc
    vtkCommonSystem
    vtkCommonTransforms
    vtkFiltersCore
    vtkFiltersExtraction
    vtkFiltersGeneral
    vtkFiltersHybrid
    vtkFiltersImaging
    vtkFiltersModeling
    vtkFiltersSources
    vtkFiltersStatistics
    vtkGUISupportQt
    vtkIOCore
    vtkIOGeometry
    vtkIOImage
    vtkIOInfovis
    vtkIOXML
    vtkIOXMLParser
    vtkImagingColor
    vtkImagingCore
    vtkImagingFourier
    vtkImagingGeneral
    vtkImagingHybrid
    vtkImagingSources
    vtkInfovisCore
    vtkInfovisLayout
    vtkInteractionStyle
    vtkInteractionWidgets
    vtkRenderingAnnotation
    vtkRenderingCore
    vtkRenderingVolumeOpenGL
    vtkRenderingLabel
    vtkRenderingOpenGL
    vtkRenderingVolume
    vtkRenderingFreeTypeOpenGL
    vtkRenderingFreeType
    vtkViewsCore
    vtkViewsInfovis
    vtkViewsQt
    vtkalglib
    vtkfreetype
    vtkzlib
    vtkftgl
    vtkChartsCore 
    vtkIOPLY
  )
  if( NOT ShapePopulationViewer_BUILD_SLICER_EXTENSION )
    list( APPEND Slicer_VTK_COMPONENTS vtkIOExport )
  endif()
endif()


find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})


find_package(VTK REQUIRED COMPONENTS ${Slicer_VTK_COMPONENTS} )
message(STATUS ${VTK_USE_FILE})
include(${VTK_USE_FILE})


SET(CREA_CMAKE_DIR ${PROJECT_SOURCE_DIR}/src/cmake)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
INCLUDE(${CREA_CMAKE_DIR}/UserMacros.cmake)

CREA_CPDIR(${PROJECT_SOURCE_DIR}/src/data ${PROJECT_BINARY_DIR}/data)


INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src/lib/Qt/SurfaceRenderer
    ${PROJECT_SOURCE_DIR}/src/lib/Qt/Viewers
    ${PROJECT_SOURCE_DIR}/src/lib/Qt/VolumeRenderer
    ${PROJECT_SOURCE_DIR}/src/lib/VTKObjects/SurfaceRenderer
    ${PROJECT_SOURCE_DIR}/src/lib/VTKObjects/ViewerWidgets
    ${PROJECT_SOURCE_DIR}/src/lib/VTKObjects/VolumeRenderer
    ${PROJECT_SOURCE_DIR}/src/data/Icons

    ${PROJECT_BINARY_DIR}/src/lib/Qt/Viewers/
    ${PROJECT_BINARY_DIR}/src/lib/Qt/VolumeRenderer/
    ${PROJECT_BINARY_DIR}/src/lib/Qt/SurfaceRenderer/
)

LINK_DIRECTORIES(
    ${PROJECT_BINARY_DIR}/lib/
)

SUBDIRS(lib)
SUBDIRS(appli)
SUBDIRS(install)


OPTION(BUILD_TEST "Build test ?" OFF)
IF(BUILD_TEST)
	add_subdirectory(test)
ENDIF(BUILD_TEST)
