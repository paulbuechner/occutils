set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_ENV_PASSTHROUGH PATH)

if (PORT MATCHES "opencascade")
  set(VCPKG_LIBRARY_LINKAGE dynamic)
  set(VCPKG_POLICY_DLLS_WITHOUT_LIBS enabled)
endif ()

set(VCPKG_CMAKE_SYSTEM_NAME MinGW)
