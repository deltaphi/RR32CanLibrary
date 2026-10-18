# Agent instructions

- This repository is the RR32Can embedded-oriented C++14 library; production code is under `lib/`, tests under `test/`, and the TCP desktop example under `examples/tcp_station/`.
- The CMake project is a development/test harness, not the library's embedded build system. Keep builds out of the source tree (`build/` is ignored because in-tree builds interfere with PlatformIO).
- Configure and build the normal Linux harness from the repository root with `cmake --preset Linux` followed by `cmake --build --preset build-Linux`; CMake downloads GoogleTest v1.18.0 during configuration, so configure requires network access.
- Run tests with `ctest --test-dir build --output-on-failure` or `./build/RR32CanTest`; focused GoogleTest cases can use `./build/RR32CanTest --gtest_filter='SuiteName.TestName'`.
- The test target is a single executable assembled explicitly in the root `CMakeLists.txt`; adding a new test file requires adding it to that target.
- The TCP example is a separate target: `cmake --build build --target rr32can_tcp_station`, then `./build/examples/tcp_station/rr32can_tcp_station`. It connects to `127.0.0.1:15731`; edit `examples/tcp_station/main.cpp` to change the host or experiments.
- Library consumers must copy `lib/RR32Can/_RR32Can_config.h` to their project as `RR32Can_config.h` and customize it; the test and example directories provide their own configs.
- C++ formatting is governed by `.clang-format` (120-column limit). `apply-clang.sh` assumes Windows LLVM paths and `clang-format.exe`, so format changed files directly when working on other platforms.
- Use C++14 with extensions disabled; non-MSVC CMake builds enable `-Wall -Wextra -pedantic`.
