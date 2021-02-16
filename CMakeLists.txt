cmake_minimum_required (VERSION 3.6)
project (RR32CanLibraryTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT MSVC)
  add_compile_options(-Wall -Wextra -pedantic)
endif()

if (WIN32)
    # Disable Windows libc depreciation warnings for POSIX functions used in the embedded code.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

option(ENABLE_COVERAGE "enable_language measurement option add_compile_definitions coverage" OFF)

if (ENABLE_COVERAGE)
    include(CodeCoverage.cmake)
    append_coverage_compiler_flags()
endif()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

enable_testing()

# Now simply link against gtest or gtest_main as needed. Eg
include_directories("lib")
include_directories("test")
add_executable(RR32CanTest
    "lib/RR32Can/Station.cpp"
    "lib/RR32Can/Constants.cpp"
    "lib/RR32Can/Locomotive.cpp"
    "lib/RR32CanEngineDb/LocoConsumer.cpp"
    "lib/RR32CanEngineDb/LocoListConsumer.cpp"
    "lib/RR32Can/types/TurnoutTypes.cpp"
    "lib/RR32Can/types/LocomotiveTypes.cpp"
    "lib/RR32Can/messages/Data.cpp"
    "lib/RR32Can/messages/Identifier.cpp"
    "lib/RR32Can/messages/TurnoutPacket.cpp"

    "lib/RR32CanEngineDb/util/BufferManager.h"
    "lib/RR32CanEngineDb/util/BufferManager.cpp"
    "lib/RR32CanEngineDb/util/TextParser.h"
    "lib/RR32CanEngineDb/util/TextParser.cpp"
    "lib/RR32CanEngineDb/util/Crc.h"
    "lib/RR32CanEngineDb/util/Crc.cpp"
    "lib/RR32CanEngineDb/util/ConfigDataStreamParser.cpp"

    "test/integration/StationTest.cpp"
    "test/integration/EngineLibraryTest.cpp"
    "test/data/ConfigData.cpp"
    "test/RR32Can/MessagesTest.cpp"
    "test/RR32Can/BufferManagerTest.cpp"
    "test/RR32Can/TextParserTest.cpp"
    "test/RR32Can/CrcTest.cpp"
    "test/RR32Can/ConstantsTest.cpp"
    "test/RR32Can/TypesTest.cpp"
)
target_link_libraries(RR32CanTest gtest_main gmock)
add_test(NAME gtest_rr32can_test COMMAND RR32CanTest)

if (ENABLE_COVERAGE)
setup_target_for_coverage_gcovr_html(
  NAME RR32CanTest-gcovr
  EXECUTABLE ctest
  DEPENDENCIES RR32CanTest
  EXCLUDE build test
)
endif()