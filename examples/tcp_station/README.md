# UDP station example

This desktop example sends CAN-over-UDP datagrams to the host and port
configured in `TcpStationConfig.h` (port `15731`). It binds to `0.0.0.0` on the
same port to receive broadcasts on all local IPv4 interfaces. Each UDP datagram
contains exactly one 13-byte packet; datagrams of other sizes are ignored:

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

The directory and executable retain their historical `tcp_station` names.

```sh
cmake -S . -B build
cmake --build build --target rr32can_tcp_station
./build/examples/tcp_station/rr32can_tcp_station
```
