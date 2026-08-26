# Alignment, offset and padding

Offset: where the field/data "should" starts (the field/data before ends) (es. in Subprofile -> name has offset 0 (start of struct), encoderCW has offset 16 (after the 16 Bytes of name), encoderCCW has offset 16 + 4 = 20)

Alignment: where the field/data can start (uint8_t -> 1 Byte, can start anywhere, uint16_t -> 2 Byte, can start at multiple of 2 (0, 2, 4, 6, 8, 10, ...), uint32_t -> 4 Byte, can start at multiple of 4 (0, 4, 8, 16, ...))
for a struct is the max between all the alignment of the fields

```
struct Example { // Size = 8 Byte, Alignment = 4 Byte (max(1, 2, 4))
  uint8_t a; // size: 1 Byte, offset: 0, alignment: 1
  uint16_t b; // size: 2 Byte, offset: 1 + 1 (padding) = 2, alignment: 2
  uint32_t c; // size: 4 Byte, offset: 4, alignment: 4
}
```

Padding: space addedd in between variables/fields (not between element of an array, since they have all the same size/alignment, but can be added before the array)

for every field:
if offset % alignment == 0 -> ok (no padding) else padding

es. (in Profile):

- name -> offset = 0, alignment = 1 Byte (char) -> ok
- subprofileCount -> offset = 16, alignment = 1 Byte -> ok
- subprofiles -> offset = 17, alignment =

# Notes:

In rust:
// device.write(&[3u8, /* dati config */])?; // byte 0 = report ID

// // Read response
// let mut buf = [0u8; 65];
// device.read(&mut buf)?;

// ------------------------------------

Default config: salvata nella RAM, se flash libera -> copia nella flash, altrimenti no (usa quella nella flash) -> per evitare overwrite delle config custom all'update del firmware

flusso tipo qmk/via:

- chiama tud_task()
- legge input
- invia HID keyboard
- riceve config via callback
- salva in flash
