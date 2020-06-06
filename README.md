# RR32CanLibrary

A library to create and parse Maerklin CAN protocol messages.

This software is written in C++ and intended to be used on embedded devices such as the ESP32.

![Windows CI](https://github.com/deltaphi/RR32CanLibrary/workflows/Windows%20CI/badge.svg?branch=master&event=push)
![Linux CI](https://github.com/deltaphi/RR32CanLibrary/workflows/Linux%20CI/badge.svg?branch=master&event=push)

### Configuration

This library has a template for a configuration header that must be copied to your project.

This library also has a nmber of callbacks that have to be implemented to achieve functionality.

### Unit Testing

This project includes a CMake project.
The CMake project is purely used to run unit tests for some parts of the code on your development machine.
You do not need to deal with unit tests (or the CMake project) unless you want to actively develop the software.
To use the CMake project, create an out-of-tree build directoy (in-tree build will confuse the Automagic of PlatformIO).
Unit testing should be possible using any C++14 capable compiler on any Desktop Operating System.
It has been tested successefully using MSVC 2017 Community Edition.

## Software Design

* Station class represents a single node on the CAN bus.
* Station can be fed CAN frames and will decode them.
* Station offers methods to be called to cause sending of messages on the CAN bus.
* Station can be configured with callbacks:
  * Transmission to CAN
  * Decoded individual CAN messages
  * Decoder for configuration data streams
* Station also offers a loco database. LocoDB can be used independently of Station class to allow sharing of one LocoDB between multiple Stations on the same processor.

### Use Cases

* Turnout Control -> AccessoryCbk
* System Status -> SystemControlCbk
* Engine Control -> EngineControlCbk
* Engine Database Management -> Configuration Stream Download (configurable consumer)