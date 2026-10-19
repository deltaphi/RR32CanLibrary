# UDP station example

This desktop example sends CAN-over-UDP datagrams to the host and port
configured in `UdpStationConfig.h` (port `15731`). It binds to `0.0.0.0:15730`
to receive broadcasts on all local IPv4 interfaces (`kReceivePort` in the same config). Each UDP datagram
contains exactly one 13-byte packet; datagrams of other sizes are ignored:

```text
4-byte CAN identifier, 1-byte DLC, 8-byte payload
```

The program logs every packet in both directions. It also uses the library's
identifier and payload formatting and passes received packets through
`RR32Can::Station` for protocol decoding.

The example uses `ConfigDataStreamParser` with `StructuredConfigDataCallback`
to write parsed data to `lokliste-test.toml`. Each parsed item becomes a TOML
array-of-tables entry, preserving order and repeated keys. All values are strings:

```toml
[[entries]]
section = "lokliste"
key = "name"
value = "DHG 500"
```

An empty output filename selects console output. Output files must not already exist.

Edit `UdpStationConfig.h` to change the gateway, or edit `runExperiments()` in
`main.cpp` to try library operations. Press `CTRL-D` on an interactive terminal
to request a graceful shutdown; the socket is then closed by the connection's
destructor.

Build from the repository root:

```sh
cmake -S . -B build
cmake --build build --target rr32can_udp_station
./build/examples/udp_station/rr32can_udp_station
```
