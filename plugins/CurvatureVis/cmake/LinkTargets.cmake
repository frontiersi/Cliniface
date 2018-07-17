if(WITH_TESTUTILS)
    target_link_libraries( ${PROJECT_NAME} ${TestUtils_LIBRARIES})
endif()

if(WITH_FACETOOLS)
    target_link_libraries( ${PROJECT_NAME} ${FaceTools_LIBRARIES})
endif()

if(WITH_QTOOLS)
    target_link_libraries( ${PROJECT_NAME} ${QTools_LIBRARIES})
endif()

if(WITH_RVTK)
    target_link_libraries( ${PROJECT_NAME} ${rVTK_LIBRARIES})
endif()

if(WITH_RPASCALVOC)
    target_link_libraries( ${PROJECT_NAME} ${rPascalVOC_LIBRARIES})
endif()

if(WITH_RLEARNING)
    target_link_libraries( ${PROJECT_NAME} ${rLearning_LIBRARIES})
endif()

if(WITH_RMODELIO)
    target_link_libraries( ${PROJECT_NAME} ${rModelIO_LIBRARIES})
endif()

if(WITH_RFEATURES)
    target_link_libraries( ${PROJECT_NAME} ${rFeatures_LIBRARIES})
endif()

if(WITH_RLIB)
    target_link_libraries( ${PROJECT_NAME} ${rlib_LIBRARIES})
endif()

if(WITH_ASSIMP)
    target_link_libraries( ${PROJECT_NAME} ${ASSIMP_LIBRARIES})
endif()

if(WITH_TINYXML)
    target_link_libraries( ${PROJECT_NAME} ${tinyxml_LIBRARY})
endif()

if(WITH_BOOST)
    target_link_libraries( ${PROJECT_NAME} Boost::regex Boost::system Boost::filesystem Boost::thread Boost::random)
endif()

if(WITH_OPENCV)
    target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS})
endif()

if(WITH_VTK)
    target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES})
endif()

if(WITH_QT)
    target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES})
endif()

if(WITH_CGAL)
    target_link_libraries( ${PROJECT_NAME} ${CGAL_LIBRARIES})
endif()

if(WITH_DLIB)
    target_link_libraries( ${PROJECT_NAME} ${dlib_LIBRARIES})
endif()
