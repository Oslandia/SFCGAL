find_package( Qt4 COMPONENTS QtGui QtCore REQUIRED )
find_package( OpenSceneGraph COMPONENTS osgQt osgViewer osgGA osgDB osgUtil REQUIRED )
find_package( PostgreSQL REQUIRED )

include_directories(
        include
        # postgresql
        ${POSTGRESQL_INCLUDE_DIR}
        ${QT_INCLUDES} 
        ${OPENSCENEGRAPH_INCLUDE_DIRS} 
)

file( GLOB_RECURSE VIEWER_SFCGAL_HEADERS ./*.h )
file( GLOB_RECURSE VIEWER_SFCGAL_HEADERS_MOC ./include/SFCGAL/*.h )
file( GLOB_RECURSE VIEWER_SFCGAL_SOURCES ./*.cpp )

QT4_WRAP_CPP(VIEWER_SFCGAL_MOC ${VIEWER_SFCGAL_HEADERS_MOC})

add_executable( viewer-SFCGAL 
        ${VIEWER_SFCGAL_HEADERS}
        ${VIEWER_SFCGAL_SOURCES}
        ${VIEWER_SFCGAL_MOC}
)

set(QT_LIBRARIES QtCore QtGui)
set_target_properties( viewer-SFCGAL PROPERTIES DEBUG_POSTFIX "d" )
target_link_libraries( viewer-SFCGAL
        SFCGAL
        ${OPENSCENEGRAPH_LIBRARIES}  
        ${QT_LIBRARIES}
        ${CGAL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${MPFR_LIBRARIES}
        ${GMP_LIBRARIES}
        # Postgresql
        pq
)
install( TARGETS viewer-SFCGAL DESTINATION bin )


