SECTION "", ROM0[$0150]

  ld a, 42
  ld b, 43
  ld c, 44
  ld d, 45
  ld e, 46
  ld h, 47
  ld l, 48

  ld a, b
  ld b, c
  ld c, d
  ld d, e
  ld e, h
  ld h, l
  ld l, a

  ld bc, 512
  ld de, 513
  ld h, 0xe0
  ld l, 0x00

  ld [hl], d
  ld a, [hl]

  ld [hl], 39
  ld b, [hl]
