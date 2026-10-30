################# breezestyle target #################
set(breezecommon_LIB_SRCS
    breeze.cpp
    breezeboxshadowrenderer.cpp
    colortools.cpp
    decorationbuttoncolors.cpp
    decorationcolors.cpp
    decorationexceptionlist.cpp
    geometrytools.cpp
    presetsmodel.cpp
    renderdecorationbuttonicon.cpp
    renderdecorationbuttonicon18by18.cpp
    styleark.cpp
    stylekisweet.cpp
    stylearkopal.cpp
    stylearkopalleft.cpp
    styleklasse.cpp
    stylesilver.cpp
    styleoxygen.cpp
    stylesuessigsilver.cpp
    styletraditional.cpp
    stylemetro.cpp
    stylefluent.cpp
    systemicontheme.cpp
    setqdebug_logging.h
)

if(HAVE_QTDBUS)
    list(APPEND breezecommon_LIB_SRCS
        dbusupdatenotifier.cpp
        dbusmessages.h
    )
endif()

kconfig_add_kcfg_files(breezecommon_LIB_SRCS breezesettings.kcfgc)

add_library(silvercommon${QT_MAJOR_VERSION} ${breezecommon_LIB_SRCS})

generate_export_header(silvercommon${QT_MAJOR_VERSION}
    BASE_NAME breezecommon
    EXPORT_FILE_NAME breezecommon_export.h)

target_link_libraries(silvercommon${QT_MAJOR_VERSION}
    PUBLIC
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Gui
        Qt${QT_MAJOR_VERSION}::Widgets

        KF${QT_MAJOR_VERSION}::GuiAddons
        KF${QT_MAJOR_VERSION}::ConfigCore
        KF${QT_MAJOR_VERSION}::ConfigGui
        KF${QT_MAJOR_VERSION}::IconThemes
        KF${QT_MAJOR_VERSION}::I18n)

if(HAVE_QTDBUS)
    target_link_libraries(silvercommon${QT_MAJOR_VERSION}
    PUBLIC
        Qt${QT_MAJOR_VERSION}::DBus
    )
    target_compile_definitions(silvercommon${QT_MAJOR_VERSION} PRIVATE HAVE_QTDBUS=1)
else()
    target_compile_definitions(silvercommon${QT_MAJOR_VERSION} PRIVATE HAVE_QTDBUS=0)
endif()

target_link_libraries(silvercommon${QT_MAJOR_VERSION} PUBLIC KF6::ColorScheme)

set_target_properties(silvercommon${QT_MAJOR_VERSION} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME silvercommon${QT_MAJOR_VERSION})

install(TARGETS silvercommon${QT_MAJOR_VERSION} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if(BUILD_TESTING)
    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Test)
    add_executable(presettoolsareatest autotests/presettoolsareatest.cpp)
    target_include_directories(presettoolsareatest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(presettoolsareatest PRIVATE silvercommon${QT_MAJOR_VERSION} Qt6::Test)
    add_test(NAME silver-presettoolsarea COMMAND presettoolsareatest)
endif()
