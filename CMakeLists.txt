cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

#CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# CPack
if(CMAKE_HOST_SYSTEM MATCHES Linux)
    add_compile_options(
        -Werror
        -Wall
    )
endif()

project(calculator-task)

# Qt
set(CMAKE_AUTOMOC ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)

set(SOURCE_FILES__
    main.cxx
    WidgetCalculator.cxx
    WidgetCalculator.h
    IntXHelper.h
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES__})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 
Qt${QT_VERSION_MAJOR}::Gui 
Qt${QT_VERSION_MAJOR}::Widgets 
Qt${QT_VERSION_MAJOR}::Gui
)
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/intx/include/ )
# Qt

# Install
if(CMAKE_HOST_SYSTEM MATCHES Windows)
    install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    qt_generate_deploy_app_script(
    TARGET ${CMAKE_PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})

    set(RUNTIME_DLLS_FROM_QT_INSTALLATION 
        zlib1.dll
        pcre2-16.dll 
        double-conversion.dll 
        zstd.dll
        libpng16.dll
        harfbuzz.dll
        freetype.dll
        brotlidec.dll 
        bz2.dll 
        brotlicommon.dll
        )
    list(TRANSFORM RUNTIME_DLLS_FROM_QT_INSTALLATION PREPEND ${PATH_QT_INSTALLED_DLL})
    install(FILES ${RUNTIME_DLLS_FROM_QT_INSTALLATION}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS WORLD_EXECUTE WORLD_READ)

else()#DEBIAN
    set(CMAKE_INSTALL_PREFIX /opt/${CMAKE_PROJECT_NAME})

    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}
            DESTINATION bin
            PERMISSIONS WORLD_EXECUTE OWNER_WRITE OWNER_READ)

    execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/symlink)
    execute_process(COMMAND ln -s /opt/${CMAKE_PROJECT_NAME}/bin/${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/symlink/${CMAKE_PROJECT_NAME})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/symlink/calculator-task
            DESTINATION /usr/bin/
            PERMISSIONS WORLD_EXECUTE WORLD_READ)

    install(FILES calculator-task.svg
            DESTINATION /usr/share/icons/hicolor/scalable/apps/)

    install(FILES calculator-task.desktop
            DESTINATION /usr/share/applications/)

    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(CPACK_PACKAGE_CONTACT "chicherin.alexey@ya.ru")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/alec-chicherini/calculator-task")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
    set(CPACK_DEBIAN_PACKAGE_VERSION 25.03)
    set(CPACK_DEBIAN_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb)
    if(${QT_VERSION_MAJOR} EQUAL 5)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS libqt5widgets5|libqt5widgets5t64 libqt5gui5|libqt5gui5t64 libqt5core5|libqt5core5a|libqt5core5t64 )
    elseif(${QT_VERSION_MAJOR} EQUAL 6)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS libqt6widgets6|libqt6widgets6t64 libqt6gui6|libqt6gui6t64 libqt6core6|libqt6core6t64 )
    else()
        message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} not supported")
    endif()

    include(CPack)
endif()
# CPack