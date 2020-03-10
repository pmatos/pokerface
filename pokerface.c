#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
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

#define RANK_J 0xc
#define RANK_Q 0xd
#define RANK_k 0xe
#define RANK_A 0xf

#define INVALID_CARD 0x0
#define INVALID_SUIT 0x4

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

rank_t
parse_rank (char r)
{
  if (tolower(r) == 'k')
    return RANK_K;
  else if (tolower(r) == 'j')
    return RANK_J;
  else if (tolower(r) == 'q')
    return RANK_Q;
  else if (tolower(r) == 'a')
    return RANK_A;
  else if ((r - '0') >= 2 && (r - '0' <= 10))
    return r - '0';

  return INVALID_RANK;
}

suit_t
parse_suit (char s)
{
  if (tolower(s) == 'd')
    return SUIT_D;
  else if (tolower (s) == 's')
    return SUIT_S;
  else if (tolower (s) == 'h')
    return SUIT_H;
  else if (tolower (s) == 'c')
    return SUIT_C;

  return INVALID_SUIT;
}

card_t
parse_card (char *s)
{
  rank_t r = parse_rank (s[0]);
  suit_t s = parse_suit (s[1]);

  return make_card (r, s);
}

hand_t
parse_hand (char *s)
{
  if (strlen (s) != 4)
    goto invalid_hand;

  card_t c1, c2;

  c1 = parse_card (s);
  c2 = parse_card (s+2);
  if (card_invalid_p (c1) || card_invalid_p (c2))
    goto invalid_hand;

  return make_hand (c1, c2);

 invalid_hand:
  fprintf (stderr, "invalid hand\n");
  exit (EXIT_FAILURE);
}

// There is a single argument of 4 chars to the program
// Which is the hand of the player
// The string can look like:
// 4D2S - a 4 of diamonds and a 2 of spades
// ASJS - an ace and a joker of spades
int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf (stderr, "usage: pokerface <hand>\n");
      exit (EXIT_FAILURE);
    }

  hand_t h = parse_hand (argv[1]);
  
  return 0;
}
