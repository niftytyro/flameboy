SECTION "", ROM0[$0150]

  ld A, 0xf0
  ld B, 0xff
  ld C, 0x00
  ld HL, 0xc000

  ld [HL], 0xf0

  CP A, B
  CP A, C
  CP A, [HL]
  CP A, 0xf1
  CP A, 0x0f

  inc A
  dec A

  inc [HL]
  ld A, [HL]
  dec [HL]
  ld A, [HL]
