set(PROJECT glacierweather)

set(SRC
    dbadapter.cpp
    openweatherapi.cpp
    weatherapi.cpp
    weatherdata.cpp)

set(HEADERS
    dbadapter.h
    openweatherapi.h)

set(PUBLIC_HEADERS
    weatherdata.h
    weatherapi.h)

add_library(${PROJECT} SHARED ${SRC} ${HEADERS} ${PUBLIC_HEADERS})

target_link_libraries(${PROJECT}
    Qt6::Core
    Qt6::Sql
    Qt6::Positioning)

set_target_properties(${PROJECT} PROPERTIES VERSION 0.1 SOVERSION 0)
add_definitions( -DINSTALL_LIBDIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

install(TARGETS ${PROJECT}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/)

install(FILES ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/glacier-weather)
