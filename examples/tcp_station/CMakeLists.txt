cmake_minimum_required(VERSION 3.6)

add_executable(rr32can_tcp_station
    main.cpp

    ../../lib/RR32Can/Constants.cpp
    ../../lib/RR32Can/Locomotive.cpp
    ../../lib/RR32Can/Station.cpp
    ../../lib/RR32Can/types/LocomotiveTypes.cpp
    ../../lib/RR32Can/types/TurnoutTypes.cpp
    ../../lib/RR32Can/messages/Data.cpp
    ../../lib/RR32Can/messages/Identifier.cpp
    ../../lib/RR32Can/messages/TurnoutPacket.cpp
)

target_include_directories(rr32can_tcp_station
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
)

set_target_properties(rr32can_tcp_station PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if (NOT MSVC)
  target_compile_options(rr32can_tcp_station PRIVATE -Wall -Wextra -pedantic)
endif()
