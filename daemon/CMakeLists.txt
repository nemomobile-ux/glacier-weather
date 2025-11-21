set(SRC
    main.cpp)

add_executable(weatherdaemon ${SRC} ${RESOURCES}
    weatcherupdater.h weatcherupdater.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(weatherdaemon
        Qt6::Core
        Qt6::Positioning
        Glacier::App
        -lglacierweather -L../lib)

configure_file(weatcherdaemon.service.in ${CMAKE_CURRENT_BINARY_DIR}/weatcherdaemon.service @ONLY)

install(TARGETS weatherdaemon RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/weatcherdaemon.service
            DESTINATION /usr/lib/systemd/user)

install(FILES weatcherdaemon.timer
            DESTINATION /usr/lib/systemd/user)
