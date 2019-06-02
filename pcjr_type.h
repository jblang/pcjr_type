#ifndef PCJR_TYPE_H
#define PCJR_TYPE_H

#include <string.h>
#include <stdint.h>
#include <avr/io.h>

void pcjrInit(void)
{
    // See Atmega328P datasheet, section 17
    // 40 KHz carrier (16 MHz, clkdiv 8, further divided by 50 via counter)
    DDRD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    TCCR2A = (1 << WGM01) | (1 << WGM00);
    TCCR2B = (1 << WGM02) | (1 << CS01);
    OCR2A = 50;
    OCR2B = 25;
}

#define PWM_ON TCCR2A |= (1 << COM0B1)
#define PWM_OFF TCCR2A &= ~(1 << COM0B1)

// See PCjr technical reference page 2-103 through 2-105 for transmitter protocol
#define SEND_1 do { PWM_ON; _delay_us(62); PWM_OFF; _delay_us(378); } while (0)
#define SEND_0 do { PWM_OFF; _delay_us(220); PWM_ON; _delay_us(63); PWM_OFF; _delay_us(158); } while(0)

void sendCode(uint8_t key)
{
  cli();
  uint8_t parity = 0;
  SEND_1;
  for (int i = 0; i < 8; i++) {
    if (key & 1) {
      SEND_1;
      parity++; 
    } else {
      SEND_0;
    }
    key >>= 1;    
  }
  if (parity & 1)
    SEND_0;
  else
    SEND_1;
  sei();
  // PCjr technical reference says send 10 stop bits between keys (off for 4840 us).
  _delay_us(4840);
}


// See PCjr technical reference page 5-23 and 5-24 for keyboard scan codes
#define PRESS(c) sendCode(c)
#define RELEASE(c) sendCode((c) | 0x80)
#define TYPE(c) do { PRESS(c); RELEASE(c); } while (0)

#define KEY_ESC 1
#define KEY_DASH 0xc
#define KEY_EQUAL 0xd
#define KEY_BACKSPACE 0xe
#define KEY_FN 0x54
#define KEY_TAB 0xf
#define KEY_LBRACKET 0x1a
#define KEY_RBRACKET 0x1b
#define KEY_ENTER 0x1c
#define KEY_CTRL 0x1d
#define KEY_SEMICOLON 0x27
#define KEY_QUOTE 0x28
#define KEY_CURUP 0x48
#define KEY_LSHIFT 0x2a
#define KEY_COMMA 0x33
#define KEY_PERIOD 0x34
#define KEY_SLASH 0x35
#define KEY_RSHIFT 0x36
#define KEY_CURLEFT 0x4b
#define KEY_CURRIGHT 0x4d
#define KEY_ALT 0x38
#define KEY_SPACEBAR 0x39
#define KEY_CAPSLOCK 0x3a
#define KEY_INS 0x52
#define KEY_DEL 0x53
#define KEY_CURDOWN 0x50
#define KEY_PHANTOM 0x55

const uint8_t letters[] PROGMEM = {
    0x1e, // a
    0x30, // b
    0x2e, // c
    0x20, // d
    0x12, // e
    0x21, // f
    0x22, // g
    0x23, // h
    0x17, // i
    0x24, // j
    0x25, // k
    0x26, // l
    0x32, // m
    0x31, // n
    0x18, // o
    0x19, // p
    0x10, // q
    0x13, // r
    0x1f, // s
    0x14, // t
    0x16, // u
    0x2f, // v
    0x11, // w
    0x2d, // x
    0x15, // y
    0x2c, // z
};

#define KEY_LETTER(c) pgm_read_byte(letters + (c))
#define KEY_NUMBER(n) ((n) == 0 ? 0xb : (n) + 1)

#define ASCII_CTRLA 0x01
#define ASCII_CTRLZ 0x1a
#define ASCII_ESC 0x1b
#define ASCII_FS 0x1c
#define ASCII_GS 0x1d
#define ASCII_RS 0x1e
#define ASCII_US 0x1f
#define ASCII_DEL 0x7f

void typeAscii(uint8_t c)
{
  switch (c) {
  case ASCII_ESC:
    TYPE(KEY_ESC);
    break;
  case ASCII_FS:
    PRESS(KEY_CTRL);
    PRESS(KEY_ALT);
    TYPE(KEY_SLASH);
    RELEASE(KEY_ALT);
    RELEASE(KEY_CTRL);
    break;
  case ASCII_GS:
    PRESS(KEY_CTRL);
    TYPE(KEY_RBRACKET);
    RELEASE(KEY_CTRL);
    break;
  case ASCII_RS:
    PRESS(KEY_CTRL);
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(6));
    RELEASE(KEY_LSHIFT);
    RELEASE(KEY_CTRL);
    break;
  case ASCII_US:
    PRESS(KEY_CTRL);
    PRESS(KEY_LSHIFT);
    TYPE(KEY_DASH);
    RELEASE(KEY_LSHIFT);
    RELEASE(KEY_CTRL);
    break;
  case '\t':
    TYPE(KEY_TAB);
    break;
  case '\b':
    TYPE(KEY_BACKSPACE);
    break;
  case '\r':
  case '\n':
    TYPE(KEY_ENTER);
    break;
  case ASCII_DEL:
    TYPE(KEY_DEL);
    break;
  case ' ':
    TYPE(KEY_SPACEBAR);
    break;
  case '!':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(1));
    RELEASE(KEY_LSHIFT);
    break;
  case '@':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(2));
    RELEASE(KEY_LSHIFT);
    break;
  case '#':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(3));
    RELEASE(KEY_LSHIFT);
    break;
  case '$':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(4));
    RELEASE(KEY_LSHIFT);
    break;
  case '%':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(5));
    RELEASE(KEY_LSHIFT);
    break;
  case '^':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(6));
    RELEASE(KEY_LSHIFT);
    break;
  case '&':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(7));
    RELEASE(KEY_LSHIFT);
    break;
  case '*':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(8));
    RELEASE(KEY_LSHIFT);
    break;
  case '(':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(9));
    RELEASE(KEY_LSHIFT);
    break;
  case ')':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_NUMBER(0));
    RELEASE(KEY_LSHIFT);
    break;
  case '-':
    TYPE(KEY_DASH);
    break;
  case '_':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_DASH);
    RELEASE(KEY_LSHIFT);
    break;
  case '=':
    TYPE(KEY_EQUAL);
    break;
  case '+':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_EQUAL);
    RELEASE(KEY_LSHIFT);
    break;
  case '[':
    TYPE(KEY_LBRACKET);
    break;
  case '{':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_LBRACKET);
    RELEASE(KEY_LSHIFT);
    break;
  case '|':
    PRESS(KEY_ALT);
    TYPE(KEY_LBRACKET);
    RELEASE(KEY_ALT);
    break;
  case ']':
    TYPE(KEY_RBRACKET);
    break;
  case '}':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_RBRACKET);
    RELEASE(KEY_LSHIFT);
    break;
  case '~':
    PRESS(KEY_ALT);
    TYPE(KEY_RBRACKET);
    RELEASE(KEY_ALT);
    break;
  case ';':
    TYPE(KEY_SEMICOLON);
    break;
  case ':':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_SEMICOLON);
    RELEASE(KEY_LSHIFT);
    break;
  case '\'':
    TYPE(KEY_QUOTE);
    break;
  case '"':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_QUOTE);
    RELEASE(KEY_LSHIFT);
    break;
  case '`':
    PRESS(KEY_ALT);
    TYPE(KEY_QUOTE);
    RELEASE(KEY_ALT);
    break;
  case ',':
    TYPE(KEY_COMMA);
    break;
  case '<':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_COMMA);
    RELEASE(KEY_LSHIFT);
    break;
  case '.':
    TYPE(KEY_PERIOD);
    break;
  case '>':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_PERIOD);
    RELEASE(KEY_LSHIFT);
    break;
  case '/':
    TYPE(KEY_SLASH);
    break;
  case '?':
    PRESS(KEY_LSHIFT);
    TYPE(KEY_SLASH);
    RELEASE(KEY_LSHIFT);
    break;
  case '\\':
    PRESS(KEY_ALT);
    TYPE(KEY_SLASH);
    RELEASE(KEY_ALT);
    break;
  default:
    if (c >= ASCII_CTRLA && c <= ASCII_CTRLZ) {
      PRESS(KEY_CTRL);
      TYPE(KEY_LETTER(c - 1));
      RELEASE(KEY_CTRL);
    } else if (c >= '0' && c <= '9') {
      TYPE(KEY_NUMBER(c - '0'));
    } else if (c >= 'a' && c <= 'z') {
      TYPE(KEY_LETTER(c - 'a'));
    } else if (c >= 'A' && c <= 'Z') {
      PRESS(KEY_LSHIFT);
      TYPE(KEY_LETTER(c - 'A'));
      RELEASE(KEY_LSHIFT);
    }
  }
}

void typeString(String str)
{
  for (int i = 0; i < str.length(); i++) {
    typeAscii(str[i]);
  }
}

#endif
