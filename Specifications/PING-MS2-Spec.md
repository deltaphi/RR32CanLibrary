# MS2 Version Detection Using CAN Ping

This specification describes the behavior implemented in `src/clone-ms2-loco.c` (`send_can_ping()` and the receive handler for `case 0x31`). All payload byte indexes are zero-based. Hexadecimal values use the `0x` prefix.

## 1. Send a ping request

Send a Classical CAN data frame on the bus to which the MS2 is connected:

| Field | Value |
|---|---|
| Identifier format | Extended (29-bit) |
| CAN identifier | `0x00300300` |
| Data length (DLC) | `0` |
| Payload | Empty |

With SocketCAN, initialize a `struct can_frame` to zero, set `can_id` to `0x00300300 | CAN_EFF_FLAG`, and write the complete structure to the bound CAN socket. `CAN_EFF_FLAG` is a SocketCAN flag, not part of the on-wire identifier.

Equivalent command using `can-utils` (replace `can0` with the appropriate interface):

```sh
cansend can0 00300300#
```

Listen for responses from devices on the bus. The application sends this request at startup. It also sends another ping when it receives an extended frame whose identifier bits 16–23 equal `0x36`, DLC equals `5`, and `data[4]` equals `0x11` (bootloader “all GO”), to detect an MS2 connected later.

## 2. Recognize a ping response from an MS2

For each received frame:

1. Require an extended CAN identifier.
2. Extract bits 16–23 of the identifier:

   ```c
   unsigned int command_byte = (frame.can_id & 0x00FF0000UL) >> 16;
   ```

3. Require `command_byte == 0x31`, indicating a ping response.
4. Examine the last payload byte, `data[7]`. Treat the response as coming from an MS2 if:

   ```c
   data[7] >= 0x30 && data[7] <= 0x3F
   ```

The existing implementation checks only this byte for device identification; it does not check `data[6]`, the device UID, or the sender's CAN hash.

An implementation following this specification should require DLC `8` before accessing the payload. The existing application's ping-response handler omits this length check.

## 3. Obtain the version number

For an accepted MS2 response, read:

| Payload index | Meaning |
|---|---|
| `data[4]` | Major version, unsigned byte |
| `data[5]` | Minor version, unsigned byte |
| `data[7]` | MS2 identification byte, `0x30`–`0x3F` |

Format the version as two decimal integers separated by a period:

```c
printf("%u.%u", (unsigned int)data[4], (unsigned int)data[5]);
```

These bytes are ordinary binary integers, not ASCII or BCD. For example, `data[4] = 0x03` and `data[5] = 0x79` represent version **3.121**. Keep the major and minor components separate rather than treating the version as a floating-point number.

### Example

```text
Extended CAN ID: 0x00311234
DLC:            8
Payload (hex):  12 34 56 78 03 79 00 32
```

- Identifier bits 16–23 are `0x31`: a ping response.
- `data[7]` is `0x32`: within the application's MS2 range.
- `data[4]` and `data[5]` are `0x03` and `0x79`: version **3.121**.

## 4. How the application uses the result

The application selects its locomotive-list transfer mode using only the major version:

```c
trigger_data.v3x = (frame.data[4] >= 3);
```

- Major version `3` or higher: newer transfer mode (`lokliste`).
- Major version below `3`: older transfer mode (`loknamen`).

The minor version is printed in verbose output but does not affect this selection. Each matching response replaces the same global mode flag; responses are not associated with individual MS2 devices. If multiple MS2s respond, the last matching response determines the mode.
