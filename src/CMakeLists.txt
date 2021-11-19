set(SRC 
    main.cpp
    appmodel.cpp
    placesmodel.cpp
    settingsmodel.cpp
    weatherdata.cpp
)

add_executable(glacier-weather ${SRC})

target_link_libraries(glacier-weather
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Qt5::Positioning
	Glacier::App)

install(TARGETS glacier-weather RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY qml
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-weather)
