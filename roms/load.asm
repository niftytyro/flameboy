SECTION "", ROM0[$0150]

  ld a, 0x1
  ld b, 0x2
  ld c, 0x3
  ld d, 0x4
  ld e, 0x5
  ld h, 0x6
  ld l, 0x7

  ld a, b
  ld b, c
  ld c, d
  ld d, e
  ld e, h
  ld h, l
  ld l, a

  ld bc, 0x1011
  ld de, 0x1213
  ld hl, 0xe000

  ld [hl], d
  ld a, [hl]

  ld [hl], 0x20
  ld a, [hl]

  ld bc, 0xe010
  ld [bc], a

  ld a, 0x40
  ld [0xe100], a

  ld b, 0xff

  ldh [0x80], a
  ld c, [0x80]

  ldh [c], a
  ld a, [bc]
