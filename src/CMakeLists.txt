set(SRC 
    main.cpp
    dbadapter.cpp
    openweatherapi.cpp
    placesmodel.cpp
    settings.cpp
    weatherdata.cpp
    weathermodel.cpp
)

qtquick_compiler_add_resources(RESOURCES qml/glacier-weather.qrc)

add_executable(glacier-weather ${SRC} ${RESOURCES})

target_link_libraries(glacier-weather
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Qt5::Positioning
        Qt5::Sql
	Glacier::App)

install(TARGETS glacier-weather RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
