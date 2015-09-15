#----------------------------------------------------------------------------
# USER! : SET THE NAME OF YOUR LIBRARY
# (Replace 'MyLib' by your own library name)

#############################
SET ( LIBRARY_NAME   GUIQtVolumeRenderer  )
#############################

#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# CREATES A USER OPTION IN CMAKE
OPTION ( BUILD_${LIBRARY_NAME}  "Build ${LIBRARY_NAME} library ?" ON)
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
IF ( BUILD_${LIBRARY_NAME} )
#----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # BUILD LIBRARY
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY HEADERS (TO BE INSTALLED)
  # EITHER LIST ALL .h, *.txx IN CURRENT DIR USING NEXT LINE:

  FILE(GLOB ${LIBRARY_NAME}_HEADERS "*.h" "*.txx")  
  
  # OR MANUALLY LIST YOUR HEADERS WITH NEXT COMMAND
  #  SET ( ${LIBRARY_NAME}_HEADERS
  #
  #      )
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY SOURCES (TO BE COMPILED)
  # EITHER LIST ALL .cxx, *.cpp, *.cc IN CURRENT DIR USING NEXT LINE:

  FILE(GLOB ${LIBRARY_NAME}_SOURCES *.cxx *.cpp *.cc)

	#SET(qtproject_UIS
	#    main_window.ui
	#)
	FILE(GLOB ${LIBRARY_NAME}_UIS "*.ui")

	QT4_WRAP_UI(${LIBRARY_NAME}_UIS_H ${${LIBRARY_NAME}_UIS})
	QT4_WRAP_CPP(${LIBRARY_NAME}_SOURCES ${${LIBRARY_NAME}_HEADERS})

	# Don't forget to include output directory, otherwise
	# the UI file won't be wrapped!
	
	#message(FATAL_ERROR ${CMAKE_CURRENT_BINARY_DIR})
	#include_directories(${QT_QTUITOOLS_INCLUDE_DIR})
  	#Now add these generated files to the ADD_EXECUTABLE step
	# If this is NOT done, then the ui_*.h files will not be generated
        #add_executable(qtproject ${qtproject_SRCS} ${qtproject_UIS_H} )

	set(${LIBRARY_NAME}_SOURCES ${${LIBRARY_NAME}_SOURCES} ${${LIBRARY_NAME}_UIS_H})

	#FILE(GLOB UIS_NAME RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ui *.qrc)
	#FOREACH(UISFILE ${UIS_NAME})
	#  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${UISFILE} ${CMAKE_BINARY_DIR}/${UISFILE} COPYONLY )
	#ENDFOREACH(UISFILE)

	
	#CREA_CPDIR(${INPUT_DATA_DIR} ${OUTPUT_DATA_DIR})
  # OR MANUALLY LIST YOUR FILES WITH NEXT COMMAND (WITHOUT EXTENSION)
  #  SET ( ${LIBRARY_NAME}_SOURCES 
  #   
  #      )
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # LIBRARY DEPENDENCIES (LIBRARIES TO LINK WITH)
  #
  # USER! : Uncomment the Libraries you need
  #
  SET ( ${LIBRARY_NAME}_LINK_LIBRARIES
  #    ${crea_LIBRARIES}
  #    ${WXWIDGETS_LIBRARIES}
  #    ${KWWidgets_LIBRARIES}
  #   ${VTK_LIBRARIES}
  #    ${ITK_LIBRARIES}
  #    ${GDCM_LIBRARIES}
  #    ${BOOST_LIBRARIES}
    ${QT_LIBRARIES}
    ${VTK_LIBRARIES}
    KernelVolumeRenderer
  # If this library must link against other libraries 
  # USER! : Add here any extra Library you need

      )
#message(FATAL_ERROR "QT LIBS ${QT_LIBRARIES}")
  #----------------------------------------------------------------------------

  #----------------------------------------------------------------------------
  # MACRO WHICH DOES ALL THE JOB : BUILD AND INSTALL

  # USER! : The default is to create a Dynamic Library.
  # if you need to create a static library
  # comment out the following line :

   ADD_LIBRARY(${LIBRARY_NAME} STATIC  ${${LIBRARY_NAME}_SOURCES})
   TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${${LIBRARY_NAME}_LINK_LIBRARIES} )

  # and uncomment the 2 lines hereafter:


  #
  #----------------------------------------------------------------------------

  #---------------------------------------------------------------------------
ENDIF ( BUILD_${LIBRARY_NAME} )

