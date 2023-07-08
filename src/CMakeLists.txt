set(SRC 
    main.cpp
    dbadapter.cpp
    openweatherapi.cpp
    placesmodel.cpp
    settings.cpp
    weatherdata.cpp
    weathermodel.cpp
)

qt_add_resources(RESOURCES qml/glacier-weather.qrc)

add_executable(glacier-weather ${SRC} ${RESOURCES})

target_link_libraries(glacier-weather
	Qt6::Gui
	Qt6::Qml
	Qt6::Quick
	Qt6::Positioning
        Qt6::Sql
	Glacier::App)

install(TARGETS glacier-weather RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
