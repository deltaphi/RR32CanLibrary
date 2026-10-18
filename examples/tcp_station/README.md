# TCP station example

This desktop example connects to a CAN-over-TCP gateway, using the host and port
configured in `TcpStationConfig.h` (defaults to `127.0.0.1:15731`). Each network
packet is exactly 13 bytes:

```text
4-byte CAN identifier, 1-byte DLC, 8-byte payload
```

The program logs every packet in both directions. It also uses the library's
identifier and payload formatting and passes received packets through
`RR32Can::Station` for protocol decoding.

Edit `TcpStationConfig.h` to change the gateway, or edit `runExperiments()` in
`main.cpp` to try library operations. Press `CTRL-D` on an interactive terminal
to request a graceful shutdown; the socket is then closed by the connection's
destructor.

Build from the repository root:

```sh
cmake -S . -B build
cmake --build build --target rr32can_tcp_station
./build/examples/tcp_station/rr32can_tcp_station
```
