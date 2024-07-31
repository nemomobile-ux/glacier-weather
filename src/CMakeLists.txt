set(SRC 
    main.cpp
    placesmodel.cpp
    settings.cpp
    weathermodel.cpp
)

qt_add_resources(RESOURCES qml/glacier-weather.qrc)

add_executable(glacier-weather ${SRC} ${RESOURCES})
include_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(glacier-weather
	Qt6::Gui
	Qt6::Qml
	Qt6::Quick
	Qt6::Positioning
        Glacier::App
        -lglacierweather -L../lib)

install(TARGETS glacier-weather RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
