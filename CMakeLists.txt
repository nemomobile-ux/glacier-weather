cmake_minimum_required(VERSION 3.6.0)

project(glacier-weather
	VERSION 0.1.2
	DESCRIPTION "The Glacier weather")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FeatureSummary)
include(GNUInstallDirs)

set(CMAKE_INSTALL_PREFIX /usr)
set(QT_MIN_VERSION "6.0.0")
find_package(Qt6 COMPONENTS Gui Qml Quick Sql Positioning LinguistTools REQUIRED)
find_package(Glacier 0.9.0 COMPONENTS App REQUIRED)

add_subdirectory(src)
add_subdirectory(lib)

# Translations
file(GLOB TS_FILES translations/*.ts)
qt_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(glacier-weather translations)
add_dependencies(glacier-weather glacierweather)

install(FILES ${QM_FILES}
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-weather/translations)

install(FILES glacier-weather.desktop
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(DIRECTORY icons
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/glacier-weather)
