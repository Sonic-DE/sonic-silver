cmake_minimum_required(VERSION 3.16)
project(silver)
set(PROJECT_VERSION "6.7.4")
set(PROJECT_VERSION_MAJOR 6)

add_definitions( -DSILVER_VERSION="${PROJECT_VERSION}" -DSILVER_GIT_MASTER=0)

set(KF6_MIN_VERSION "6.14.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")
set(QT_MIN_VERSION "6.8.0")

include(GenerateExportHeader)
include(WriteBasicConfigVersionFile)
include(FeatureSummary)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We need this because we can't include KDECMakeSettings here, because that
# would need KDEInstallDirs, which we can only include in the qt{5,6} builds
# to get the correct installation dirs for each Qt version.
option(BUILD_TESTING "Build the testing tree." ON)
if(BUILD_TESTING)
    enable_testing()
endif()

include(CMakePackageConfigHelpers)
include(ECMInstallIcons)
include(ECMQtDeclareLoggingCategory)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(KDEGitCommitHooks)

include(GtkUpdateIconCache)

option(BUILD_QT6 "Build with Qt6" ON)

set(QT_NO_CREATE_VERSIONLESS_TARGETS ON)
set(QT_NO_CREATE_VERSIONLESS_FUNCTIONS ON)

function(build_Qt6)
    set(QT_MAJOR_VERSION 6)
    include(KDEInstallDirs6)
    include(KDECMakeSettings)

    if(UNIX AND NOT APPLE AND NOT ANDROID)
        find_package(Qt6 ${REQUIRED_QT_VERSION} CONFIG REQUIRED DBus)
        set(HAVE_QTDBUS ${Qt6DBus_FOUND})

        option(WITH_DECORATIONS "Build Silver window decorations for KWin" ON)
        option(WITH_WALLPAPERS "Install Silver default wallpapers" ON)


        find_package(KF6FrameworkIntegration ${KF6_MIN_VERSION} CONFIG )
        set_package_properties(KF${QT_MAJOR_VERSION}FrameworkIntegration PROPERTIES
            DESCRIPTION "KF6 Framework Integration"
            URL "https://projects.kde.org/projects/frameworks/frameworkintegration"
            TYPE OPTIONAL
            PURPOSE "Required to use KStyle convenience functionalities in style")

        find_package(KF6KCMUtils ${KF6_MIN_VERSION})
        set_package_properties(KF6KCMUtils PROPERTIES
            TYPE REQUIRED
            DESCRIPTION "Helps create configuration modules"
            PURPOSE "KCMUtils used for the configuration modules or the decoration and Qt Style"
        )
    else()
        set(WITH_DECORATIONS OFF)
        set(WITH_WALLPAPERS OFF)
    endif()

    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Svg Xml) #Svg and Xml only needed for settings (Qt6 only)

    find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
        CoreAddons
        ColorScheme
        Config
        GuiAddons
        I18n
        IconThemes
        WindowSystem)

    find_package(Qt6 ${QT_MIN_VERSION} OPTIONAL_COMPONENTS Quick)
    if(${Qt6Quick_FOUND})
        find_package(KF6KirigamiPlatform ${KF6_MIN_VERSION} REQUIRED)
    endif()

    set(SILVER_HAVE_KSTYLE ${KF6FrameworkIntegration_FOUND})
    set(SILVER_HAVE_QTQUICK ${Qt6Quick_FOUND})

    # https://bugreports.qt.io/browse/QTBUG-114706
    add_library(Qt::Core ALIAS Qt6::Core)
    add_library(Qt::Gui ALIAS Qt6::Gui)
    add_library(Qt::OpenGL ALIAS Qt6::OpenGL)
    add_library(Qt::Network ALIAS Qt6::Network)
    if(Qt6_VERSION_MINOR GREATER 6)
        add_library(Qt::PlatformModuleInternal ALIAS Qt6::PlatformModuleInternal)
    endif()

    add_subdirectory(kstyle kstyle6)
    add_subdirectory(libbreezecommon libbreezecommon6)

    if(WITH_DECORATIONS)
        find_package(KDecoration3 REQUIRED)

        string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ ${KDecoration3_VERSION})
        set(KDECORATION_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(KDECORATION_VERSION_MINOR ${CMAKE_MATCH_2})
        set(KDECORATION_VERSION_PATCH ${CMAKE_MATCH_3})
        add_definitions( -DKDECORATION_VERSION_MAJOR=${KDECORATION_VERSION_MAJOR} -DKDECORATION_VERSION_MINOR=${KDECORATION_VERSION_MINOR} -DKDECORATION_VERSION_PATCH=${KDECORATION_VERSION_PATCH})

        add_subdirectory(kdecoration)
        add_subdirectory(layout-templates)
        add_subdirectory(look-and-feel)
    endif()

    if(WITH_WALLPAPERS)
        add_subdirectory(wallpapers)
    endif()

    ki18n_install(po)

    # create a Config.cmake and a ConfigVersion.cmake file and install them
    include(ECMSetupVersion)
    ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX BREEZE
        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/SonicSilverConfigVersion.cmake"
    )
    set(SONICSILVER_CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/SonicSilver")

    configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/SonicSilverConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/SonicSilverConfig.cmake"
                                  PATH_VARS KDE_INSTALL_FULL_DATADIR
                                  INSTALL_DESTINATION ${SONICSILVER_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SonicSilverConfig.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/SonicSilverConfigVersion.cmake"
            DESTINATION "${SONICSILVER_CMAKECONFIG_INSTALL_DIR}"
            COMPONENT Devel
    )

    set(BREEZE_CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/Breeze")
    configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/BreezeConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/BreezeConfig.cmake"
                                  PATH_VARS KDE_INSTALL_FULL_DATADIR
                                  INSTALL_DESTINATION ${BREEZE_CMAKECONFIG_INSTALL_DIR}
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/BreezeConfig.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/SonicSilverConfigVersion.cmake"
            DESTINATION "${BREEZE_CMAKECONFIG_INSTALL_DIR}"
            COMPONENT Devel
    )

    unset(QUERY_EXECUTABLE CACHE)
endfunction()
build_Qt6()

include(KDEInstallDirs6)
add_subdirectory(colors)
add_subdirectory(cursors)

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
list(FILTER ALL_CLANG_FORMAT_SOURCE_FILES EXCLUDE REGEX ".*\\.ccls-cache/.*")
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
