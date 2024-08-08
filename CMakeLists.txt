cmake_minimum_required(VERSION 3.10)
project(DicomReader)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Add vcpkg toolchain
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Set the DCMTK_DIR
set(DCMTK_DIR "D:/vcpkg/vcpkg/packages/dcmtk_x64-windows")  # Adjust according to your installation path

# Include directories
include_directories(${DCMTK_DIR}/include)

# Link directories
link_directories(${DCMTK_DIR}/lib)

# Find DCMTK package
find_package(DCMTK REQUIRED)
include_directories(${DCMTK_INCLUDE_DIRS})
link_directories(${DCMTK_LIBRARY_DIRS})


# Find OpenCV package
find_package(OpenCV REQUIRED)

# Manually specify GTest paths
set(GTEST_ROOT "D:/vcpkg/vcpkg/installed/x64-windows")
set(GTEST_INCLUDE_DIRS "${GTEST_ROOT}/include")
set(GTEST_LIBRARIES "${GTEST_ROOT}/lib/gtest.lib;${GTEST_ROOT}/lib/manual-link/gtest_main.lib")
# Set the path to the DLL directory
set(DLL_DIR "D:/vcpkg/vcpkg/installed/x64-windows/bin")
set(VCPKG_INSTALLED_DIR "D:/vcpkg/vcpkg")


include_directories("D:/vcpkg/vcpkg/installed/x64-windows/debug/bin")
include_directories("D:/vcpkg/vcpkg/installed/x64-windows/bin")
include_directories(${GTEST_INCLUDE_DIRS})

# Find DCMTK package
find_package(DCMTK REQUIRED)
include_directories(${DCMTK_INCLUDE_DIRS})
link_directories(${DCMTK_LIBRARY_DIRS})

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source files
set(SOURCES
    src/main.cpp
    src/DicomReader.cpp
)

# Add executable
add_executable(DicomReader ${SOURCES})

# Ensure C++ standard is set for all targets
set_target_properties(DicomReader PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


# Link libraries
target_link_libraries(DicomReader
    DCMTK::dcmdata
    DCMTK::ofstd
	${DCMTK_LIBRARIES}
    ${OpenCV_LIBS})

# Add test executable
find_package(GTest REQUIRED)

enable_testing()

add_library(DicomReaderLib src/DicomReader.cpp)

add_executable(UnitTests
    test/test_main.cpp
	src/DicomReader.cpp)

target_link_libraries(UnitTests
	DCMTK::dcmdata
    DCMTK::ofstd
	${GTEST_LIBRARIES}
	${DCMTK_LIBRARIES}
	${OpenCV_LIBS} DicomReaderLib)



#Add DCMTK definitions

if ("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}" STREQUAL "/")
    message(WARNING "No vcpkg install dir found, DCMTK definitions are not set")
else()
    target_compile_definitions(DicomReader
        PUBLIC
            "DCMTK_PREFIX=${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
            "DCM_DICT_DEFAULT_PATH=${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/dcmtk-3.6.8/dicom.dic:${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/dcmtk-3.6.8/private.dic"
            "DEFAULT_CONFIGURATION_DIR=${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/etc/dcmtk-3.6.8/"
            "DEFAULT_SUPPORT_DATA_DIR=${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/dcmtk-3.6.8/"
    )
endif()



# Copy DLLs to the output directory
add_custom_command(TARGET DicomReader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${DLL_DIR}
    $<TARGET_FILE_DIR:DicomReader>)


# Manually set compiler flags for Visual Studio
if(MSVC)
    add_compile_options(/std:c++17)
endif()
# Manually set compiler flags if necessary
if(MSVC)
    target_compile_options(DicomReader PRIVATE /std:c++17)
#    target_compile_options(UnitTests PRIVATE /std:c++17)
else()
    target_compile_options(DicomReader PRIVATE -std=c++17)
#    target_compile_options(UnitTests PRIVATE -std=c++17)
endif()

