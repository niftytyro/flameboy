SECTION "", ROM0[$0150]

  ld a, 0xf0
  ld b, 0x0f
  sub a, b
  add a, b

  ld HL, 0xc000
  ld [HL], 0x1

  sub a, [HL]

  sub a, 0xe0
  sub a, 0x01

  sbc a, 0xff
  
  ld a, 0xff
  ld b, 0xff
  ld [HL], 0x1

  sbc a, b
  sbc a, [HL]

