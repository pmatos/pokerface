#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// A deck of cards is represented by 52 uint8_t.
// The suit is represented by the bottom 2 bits.
// 00 hearts
// 01 diamonds
// 10 spades
// 11 clubs
// black suits 1x
// red suits   0x
//
// The rank representation needs 4 more bits
// 0000 invalid
// 0001 invalid
// 0010 2
// 0011 3
// 0100 4
// 0101 5
// 0110 6
// 0111 7
// 1000 8
// 1001 9
// 1010 10
// 1011 invalid
// 1100 J
// 1101 Q
// 1110 K
// 1111 A

typedef uint8_t card_t;
typedef uint8_t rank_t;
typedef uint8_t suit_t;
typedef struct {
  card_t deck[52];
} deck_t;

typedef uint16_t hand_t;

inline bool
card_hearts_p (card_t c)
{
  return (c & 0x3) == 0x0;
}

inline bool
card_diamonds_p (card_t c)
{
  return (c & 0x3) == 0x1;
}

inline bool
card_spades_p (card_t c)
{
  return (c & 0x3) == 0x2;
}

inline bool
card_clubs_p (card_t c)
{
  return (c & 0x3) == 0x3;
}

inline bool
card_red_p (card_t c)
{
  return (c & 0x2) == 0;
}

inline bool
card_black_p (card_t c)
{
  return (c & 0x2) == 1;
}

inline bool
card_invalid_p (card_t c)
{
  uint8_t v = c >> 2;
  return v == 0x0 || v == 0x1 || v == 0xb || v > 0xf;
}

inline hand_t
make_hand (card_t c1, card_t c2)
{
  if (c1 < c2)
    return c1 << 0x8 | c2;
  else
    return c2 << 0x8 | c1;
}

inline card_t
hand_card1 (hand_t h)
{
  return h >> 0x8;
}

inline card_t
hand_card2 (hand_t h)
{
  return h & 0xf;
}

