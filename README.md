# RR32CanLibrary

A library to create and parse Maerklin CAN protocol messages.

This software is written in C++ and intended to be used on embedded devices.
It was originally written for the ESP32 but has also been successfully used in an STM32F1.
There is nothing in the software that prevents you from also using it in desktop software.
Finally, the software has also been squeezed into an Atmega328. However, there are next to no resources left in the Atmega328 - YMMV.

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

### Multi Threading

Initialization must occur in a single-threaded mode.

Currently, RR32CANLibrary is not equipped for multi threading. Note that many things are stateless, especially Turnout control.
Statefulness is present in:
* Locomotives (Records state information off of the bus),
* Configuration Data Consumption (aggregate configuration data off of the bus),
* the Loco Database (which loco objects are currently in memory).

To achieve thread safety, what likely needs to be done:
* Bolt down Locomotives with locking.
* Make sure that the Loco Database is consistent and that Loco objects in use cannot be deleted. As Loco Download is a two-stage process (brows the list, then download actual locos) this is likely to be easy to solve: Remove Loco object storage from the library, leave this to the outside world and just keep a pointer to "the active loco". (TBD: Investigate how to track state of multiple locos).
* For confoguration data, you can likely get around locking, as there is only one bus that feeds the configuration download.

To ensure operation on all systems (ATmega328, STM32F1, ESP32) it should be configurable on a per-project basis whether locking is used and what primitives are used for locking.
