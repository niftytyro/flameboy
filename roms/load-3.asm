SECTION "", ROM0[$0150]

  ld hl, 0xc000

  ld [hl], 0x42
  ld a, 84

  ld [hli], a

  ld [hl], 0x43
  ld [hld], a

  ld [hl], 0x44
  ld a, [hli]

  ld [hl], 0x45
  ld a, [hld]
