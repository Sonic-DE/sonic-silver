add_definitions(-DTRANSLATION_DOMAIN="silver_kwin_deco")

################# includes #################
include_directories(${CMAKE_BINARY_DIR}/libbreezecommon6)

################# newt target #################
### plugin classes
set(breezedecoration_SRCS
    breezebutton.cpp
    breezedecoration.cpp
    breezesettingsprovider.cpp
    breezetitlebarmenuitem.cpp
)

### build library
#This is removed as kdecoration InternalSettings is now generated from libbreezecommon
#kconfig_add_kcfg_files(silverdecoration_STATIC breezesettings.kcfgc)
add_library(silverdecoration MODULE
    ${breezedecoration_SRCS}
    ${breezedecoration_config_PART_FORMS_HEADERS})

set_target_properties(silverdecoration PROPERTIES
    OUTPUT_NAME org.kde.silver
)

target_link_libraries(silverdecoration
    PRIVATE
        silvercommon6
        Qt6::DBus
        KF6::CoreAddons
        KF6::ConfigGui
        KF6::GuiAddons
        KF6::I18n
        KF6::IconThemes
        KF6::WindowSystem
        KDecoration3::KDecoration
        KDecoration3::TitleBarMenu
        Sonic::Logging
)

install(TARGETS silverdecoration DESTINATION ${KDE_INSTALL_PLUGINDIR}/${KDECORATION_PLUGIN_DIR})

add_subdirectory(config)
