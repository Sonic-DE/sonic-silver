### configuration plugin
add_definitions(-DTRANSLATION_DOMAIN=\"silver_style_config\")
set(kstyle_breeze_config_PART_SRCS breezestyleconfig.cpp ui/silverstyleconfig.qrc)

set(kstyle_breeze_config_PART_SRCS ${kstyle_breeze_config_PART_SRCS} breezestyleconfigmodule.cpp )

kconfig_add_kcfg_files(kstyle_breeze_config_PART_SRCS ../breezestyleconfigdata.kcfgc)
set(kstyle_breeze_config_PART_FORMS ui/breezestyleconfig.ui)
ki18n_wrap_ui(kstyle_breeze_config_PART_FORMS_HEADERS ${kstyle_breeze_config_PART_FORMS})

set(CMAKE_AUTORCC ON)

kcoreaddons_add_plugin(silverstyleconfig SOURCES
    ${kstyle_breeze_config_PART_SRCS}
    ${kstyle_breeze_config_PART_FORMS_HEADERS}
    INSTALL_NAMESPACE "kstyle_config"
)

target_link_libraries(silverstyleconfig
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

if(HAVE_QTDBUS)
    target_link_libraries(silverstyleconfig
        PUBLIC
            Qt6::DBus
    )
endif()

target_link_libraries(silverstyleconfig
    PUBLIC
        KF6::CoreAddons
        KF6::ConfigCore
        KF6::ConfigWidgets
        KF6::I18n
)
if(QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(silverstyleconfig
        PUBLIC
        KF6::KCMUtils
    )
endif()
kcmutils_generate_desktop_file(silverstyleconfig)

include_directories(${CMAKE_SOURCE_DIR}/libbreezecommon)
include_directories(${CMAKE_BINARY_DIR}/libbreezecommon6)


set(silver_settings_SOURCES
    main.cpp
)
add_executable(silver-settings ${silver_settings_SOURCES} )
target_link_libraries(silver-settings Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg Qt6::Xml)
target_link_libraries(silver-settings KF6::I18n KF6::KCMUtils KF6::CoreAddons)
target_link_libraries(silver-settings silvercommon6)

install(TARGETS silver-settings ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

# icon
ecm_install_icons(ICONS sc-apps-silver-settings.svgz
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor )

# .desktop file
install(FILES silver-settings.desktop DESTINATION ${KDE_INSTALL_APPDIR})
