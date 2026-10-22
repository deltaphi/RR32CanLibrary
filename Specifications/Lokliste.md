# `lokliste` data format

## Scope and sources

This specification describes the payload in `lokliste-test.cs2`, beginning at
the `[lokliste]` marker. Bytes before that marker are not part of the format
and must be ignored.

The Märklin *CAN CS2 Protokoll 2.0* (7 February 2012) was used as additional
reference, especially:

- §1.3.1, UID and Loc-ID definitions;
- §7.1–7.2, configuration requests and stream transport;
- §8.1.3, locomotive configuration fields;
- §8.4–8.5, `lokinfo` and `loknamen` streams.

The reference document does not directly document the `lokliste`, `llindex`,
`idx`, `dv`, or textual `crc` fields. Their meanings below are therefore based
on the observed sample where explicitly noted.

## Overall structure

The payload is a line-oriented, section-based text format:

```text
[lokliste]
.dv=1021
.idx=0
.llindex=0
.name=DHG 500
.crc=136
.uid=0xc032
.mfxuid=0x1
.idx=1
.llindex=1
.name=VT89 Neu
.crc=99
.uid=0xc04f
.mfxuid=0x1
```

There is one `[lokliste]` section, followed by section metadata and a sequence
of locomotive records. Records have no explicit closing marker; a record ends
when the next record starts or when the payload ends.

## Lexical rules

- Each assignment occupies one line and has the form `.key=value`.
- Leading indentation is insignificant and is not a record delimiter.
- Keys are lowercase in the observed format and have one leading dot.
- Values are unquoted. A name may contain spaces and punctuation.
- Decimal values have no prefix; hexadecimal values use the `0x` prefix.
- Split at the first `=` so that the remainder of the line is preserved as the
  value.
- Whitespace-only lines may occur.
- The sample defines no escaping convention, maximum name length, or character
  encoding. Receivers should preserve name text rather than normalizing it.

## Section metadata

### `[lokliste]`

The section marker identifies the locomotive-list payload.

### `.dv`

`.dv` occurs once before the records. The sample contains `.dv=1021`. It is
list-level metadata, but its exact meaning is undocumented. It may be a data
format/version value; this must not be assumed without further captures.

## Locomotive records

The observed record shape is:

```text
.idx=<decimal>
.llindex=<decimal>
.name=<text>
.crc=<decimal>
.uid=<hexadecimal>
.mfxuid=<hexadecimal>
```

Every record in the sample contains these fields in this order. This single
capture does not prove that all fields are mandatory or that the order is
fixed.

| Field | Observed value | Provisional meaning |
|---|---|---|
| `.idx` | `0`–`7` | Metadata associated with the following record. It equals `.llindex` in the sample, but its independent meaning is unknown. |
| `.llindex` | `0`–`7` | Apparent locomotive-list index and record-start marker. |
| `.name` | Unquoted text | Locomotive display name. This corresponds to the locomotive name concept in §8.1.3 of the protocol. |
| `.crc` | Decimal, `71`–`215` in the sample | Record checksum or change indicator; algorithm and covered bytes are unknown. Do not confuse it with the transport CRC. |
| `.uid` | 32-bit hexadecimal value | Protocol-dependent Loc-ID. |
| `.mfxuid` | 32-bit hexadecimal value | mfx decoder UID, or an invalid/uninitialized value where no mfx UID is available. |

All numeric values should be parsed as unsigned values. In particular,
`0xffffffff` must be retained rather than treated as a signed negative value.

## `.idx` and `.llindex`

The sample contains eight `.idx` and eight `.llindex` fields. Each pair is
adjacent and has the same value:

```text
.idx=N
.llindex=N
```

There is an initial `.idx=0` and no trailing `.idx=8`. `.idx` belongs to the
following record; it does not belong to the preceding record. The first
`.idx` therefore qualifies the first `.llindex` record.

Recommended parsing behavior:

1. Store `.dv` as section metadata.
2. Store `.idx` as metadata pending for the next record.
3. Start a record at `.llindex` and attach the pending `.idx` to it.
4. Associate subsequent `.name`, `.crc`, `.uid`, and `.mfxuid` fields with that
   record.
5. Finalize the previous record when another `.llindex` is encountered.
6. Finalize the final record at end of input.

Keep `.idx` and `.llindex` as separate properties. Their equality in this
sample does not prove that they are interchangeable, always contiguous, or
stable when the list changes. Their ownership is nevertheless unambiguous:
both values describe the same following record.

## Sample decoded records

| `.idx` | `.llindex` | `.name` | `.crc` | `.uid` | `.mfxuid` |
|---:|---:|---|---:|---|---|
| 0 | 0 | `DHG 500` | 136 | `0xc032` | `0x1` |
| 1 | 1 | `VT89 Neu` | 99 | `0xc04f` | `0x1` |
| 2 | 2 | `BR 24 058` | 215 | `0xc018` | `0x1` |
| 3 | 3 | `BR2160901V5.0M4` | 99 | `0x4007` | `0xfa1a3b1e` |
| 4 | 4 | `38 2208` | 87 | `0x4005` | `0xf9ef069a` |
| 5 | 5 | `BR 81 004` | 209 | `0x4006` | `0x73e48c58` |
| 6 | 6 | `Lokliste` | 71 | `0x1` | `0xffffffff` |
| 7 | 7 | `BR 216` | 79 | `0xc051` | `0xffffffff` |

According to the Loc-ID ranges in §1.3.1 of the protocol:

- `0x4000`–`0x7fff` is the mfx range;
- `0xc000`–`0xffff` is the DCC range;
- the lower ranges include Märklin Motorola addresses.

Thus, for example, `0xc032` is a DCC Loc-ID for address `0x32`, while
`0x4007` is an mfx Loc-ID with SID `7`. This classification is based on `.uid`
and does not depend on `.mfxuid`; the sample contains nonzero `.mfxuid` values
on entries whose `.uid` is in the DCC range.

The name `Lokliste` in record 6 is an ordinary `.name` value, not a new section
marker.

## Transport and CRC

The protocol's Config Data Stream (§7.2) transports configuration data as an
initial frame containing stream length and a 16-bit transport CRC, followed by
8-byte data frames. The final frame may contain zero padding. A parser should
assemble the meaningful payload before parsing lines and must not mistake
transport padding for text.

The documented transport CRC uses initial value `0xffff` and polynomial
`0x1021`. This does **not** establish the algorithm for the textual `.crc`
field, which is a separate, per-record value in the sample.

The reference specifies zlib compression for some named configuration streams,
but does not establish whether this particular `lokliste` payload is
compressed. Compression must therefore be determined by the surrounding
transport/request implementation, not by this text format alone.

## Receiver requirements and recommendations

- Find `[lokliste]` before interpreting assignments; ignore all preceding data.
- Parse records rather than storing the section as a single key/value map.
- Accept arbitrary leading whitespace and whitespace-only lines.
- Support a record ending at end-of-input without a following `.llindex`.
- Preserve unknown fields for forward compatibility; the protocol document
  explicitly allows undocumented extensions.
- Do not assume exactly eight records or contiguous indices.
- Do not require `.idx` and `.llindex` to be equal in future data.
- Preserve raw hexadecimal values and distinguish `0`, `0xffffffff`, and a
  missing field.
- Treat invalid numeric syntax as malformed input rather than silently turning
  it into zero.

## Unresolved questions

Further captures or documentation are required to determine:

- the exact meanings of `.dv` and `.idx`;
- whether `.idx` and `.llindex` can differ;
- the `.crc` algorithm and the bytes it covers;
- mandatory fields and permitted field ordering;
- empty-list and partial-list representations;
- name encoding, escaping, and length limits;
- pagination or request semantics; and
- whether this payload can be compressed.
