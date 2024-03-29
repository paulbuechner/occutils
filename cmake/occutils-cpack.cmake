set(CPACK_GENERATOR "TGZ;ZIP" CACHE STRING "Semicolon separated list of generators")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}" "${PROJECT_NAME}" ALL .)

set(CPACK_PACKAGE_NAME "occutils")
set(CPACK_PROJECT_URL "https://github.com/paulbuechner/occutils")
set(CPACK_PACKAGE_VENDOR "Paul Büchner")
set(CPACK_PACKAGE_CONTACT "Paul Büchner <paul-buechner_86432180@outlook.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A utility library providing high-level functions and algorithms to empower OpenCASCADE-based engineering tasks.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
if (PROJECT_VERSION_TWEAK)
  set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}.${PROJECT_VERSION_TWEAK})
endif ()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/packages")
set(CPACK_PACKAGE_RELOCATABLE ON CACHE BOOL "Build relocatable package")

set(CPACK_RPM_PACKAGE_LICENSE "Apache-2.0")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")

if (NOT CPACK_PACKAGE_RELOCATABLE)
  # Depend on pkgconfig rpm to create the system pkgconfig folder
  set(CPACK_RPM_PACKAGE_REQUIRES pkgconfig)
  set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION
      "${CPACK_PACKAGING_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif ()

include(CPack)
