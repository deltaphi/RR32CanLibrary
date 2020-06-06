cmake_minimum_required (VERSION 3.6)
project (RR32CanControllerTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
    test/RR32Can/BufferManagerTest.cpp
    test/RR32Can/TextParserTest.cpp
    test/RR32Can/CrcTest.cpp
    
    "lib/RR32Can/Station.cpp"
    "lib/RR32Can/Constants.cpp"
    "lib/RR32Can/Locomotive.cpp"
    "lib/RR32Can/LocoConsumer.cpp"
    "lib/RR32Can/LocoListConsumer.cpp"
    "lib/RR32Can/Types.cpp"
    "lib/RR32Can/messages/Data.cpp"
    "lib/RR32Can/messages/Identifier.cpp"
    "lib/RR32Can/messages/TurnoutPacket.cpp"
    lib/RR32Can/util/BufferManager.h
    lib/RR32Can/util/BufferManager.cpp
    lib/RR32Can/util/TextParser.h
    lib/RR32Can/util/TextParser.cpp
    lib/RR32Can/util/Crc.h
    lib/RR32Can/util/Crc.cpp
    "lib/RR32Can/util/utils.cpp"
    "lib/RR32Can/util/ConfigDataStreamParser.cpp"
    "test/integration/StationTest.cpp"
    "test/integration/EngineLibraryTest.cpp"
    "test/data/ConfigData.cpp"
    )
target_link_libraries(RR32CanTest gtest_main gmock)
add_test(NAME gtest_rr32can_test COMMAND RR32CanTest)
