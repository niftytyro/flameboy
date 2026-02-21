SECTION "", ROM0[$0150]

  ld a, 0xf0
  ld b, 0x0f
  add a, b

  ld HL, 0xc000
  ld [HL], 0x1

  add a, [HL]

  adc a, 0xf0
  add a, 0x10

  adc a, 0x01
  ld b, 0xff
  ld [HL], 0x1
  
  adc a, b
  adc a, [HL]

