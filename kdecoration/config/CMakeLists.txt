### config classes
set(breezedecoration_config_SRCS
    breezeconfigwidget.cpp
    breezedetectwidget.cpp
    breezeexceptiondialog.cpp
    breezeexceptionlistwidget.cpp
    breezeexceptionmodel.cpp
    breezeitemmodel.cpp
    buttonsizing.cpp
    windowoutlinestyle.cpp
    loadpreset.cpp
    addpreset.cpp
    buttoncolors.cpp
    buttonbehaviour.cpp
    titlebarspacing.cpp
    titlebaropacity.cpp
    shadowstyle.cpp
)

ki18n_wrap_ui(breezedecoration_config_SRCS
   ui/breezeconfigurationui.ui
   ui/breezeexceptiondialog.ui
   ui/breezeexceptionlistwidget.ui
   ui/buttonsizing.ui
   ui/windowoutlinestyle.ui
   ui/loadpreset.ui
   ui/addpreset.ui
   ui/buttoncolors.ui
   ui/buttonbehaviour.ui
   ui/titlebarspacing.ui
   ui/titlebaropacity.ui
   ui/shadowstyle.ui
)

qt6_add_resources(breezedecoration_config_SRCS ui/silverdecoration_config.qrc)

kcoreaddons_add_plugin(kcm_silverdecoration SOURCES kcm_breezedecoration.cpp ${breezedecoration_config_SRCS} INSTALL_NAMESPACE "${KDECORATION_KCM_PLUGIN_DIR}")
#This is removed as kdecoration InternalSettings is now generated from libbreezecommon
#kconfig_add_kcfg_files(silverdecoration_STATIC breezesettings.kcfgc)
target_include_directories(kcm_silverdecoration PRIVATE ${CMAKE_SOURCE_DIR}/kdecoration ${CMAKE_BINARY_DIR}/kdecoration/)
target_link_libraries(kcm_silverdecoration
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::DBus
        Qt6::Widgets
        KDecoration3::KDecoration
    PRIVATE
        KF6::ConfigCore
        KF6::CoreAddons
        KF6::GuiAddons
        KF6::I18n
        KF6::IconThemes
        KF6::WindowSystem
        KF6::KCMUtils
        silvercommon6
)

kcmutils_generate_desktop_file(kcm_silverdecoration)

#generate preset files with the current project version substituted
file(GLOB PRESET_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/presets/*.klpw")
foreach(preset_file ${PRESET_FILES})
    get_filename_component(preset_name "${preset_file}" NAME)
    configure_file("${preset_file}" "${CMAKE_CURRENT_BINARY_DIR}/presets/${preset_name}" @ONLY)
endforeach()

#install bundled presets
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/presets DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/org.kde.kdecoration3.kcm/silverdecoration)
