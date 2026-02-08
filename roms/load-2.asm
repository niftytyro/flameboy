SECTION "", ROM0[$0150]

  ld A, 0x42
  ldh [0xff01], A

  ld C, 0x02
  ld A, 0x43
  ldh [C], A

  ld BC, 0xff01
  ld A, [BC]

  ld A, [0xff02]

  ld A, [0xff01]
  ld C, 0x03
  ldh [C], A

  ld A, [0xff02]
  ldh A, [C]
