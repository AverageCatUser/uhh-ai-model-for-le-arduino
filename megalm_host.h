#ifndef MEGALM_HOST_H
#define MEGALM_HOST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <chrono>

#define PROGMEM
#define F(x) (x)
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#define strlen_P strlen
#define E2END 4095

static uint8_t g_hostEep[4096];
static inline uint8_t hostEepRead(uint16_t a) { return g_hostEep[a & 4095]; }
static inline void hostEepWrite(uint16_t a, uint8_t v) { g_hostEep[a & 4095] = v; }

static inline uint32_t millis() {
  using namespace std::chrono;
  static auto t0 = steady_clock::now();
  return (uint32_t)duration_cast<milliseconds>(steady_clock::now() - t0).count();
}
static inline uint32_t micros() { return millis() * 1000UL; }
static inline void delay(uint32_t ms) { (void)ms; }

struct HostSerial {
  char buf[512];
  int  head = 0, len = 0;
  void begin(long) {}
  operator bool() const { return true; }
  void flush() { fflush(stdout); }
  int available() {
    if (head < len) return len - head;
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    len = (int)strlen(buf); head = 0;
    return len;
  }
  int read() { return (head < len) ? (uint8_t)buf[head++] : -1; }
  void print(const char *s) { fputs(s, stdout); }
  void print(char c) { fputc(c, stdout); }
  void print(int v) { printf("%d", v); }
  void print(unsigned int v) { printf("%u", v); }
  void print(long v) { printf("%ld", v); }
  void print(unsigned long v) { printf("%lu", v); }
  void print(double v, int d = 2) { printf("%.*f", d, v); }
  void println() { fputc('\n', stdout); }
  void println(double v, int d) { printf("%.*f\n", d, v); }
  template <class T> void println(T v) { print(v); fputc('\n', stdout); }
};
static HostSerial Serial;

#endif
