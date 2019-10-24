#include <stdio.h>
#include <stdlib.h>
#include "vcd_parser.h"

int stringEq (
  const unsigned char* gold,
  const unsigned char* p,
  const unsigned char* endp
) {
  for (size_t i = 0; gold[i] != 0; i++) {
    if (gold[i] != p[i]) {
      return 0;
    }
  }
  return 1;
}

int commandSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  // printf("(%d:%d:%d:%d)(%.*s)\n", s->time, s->command, s->type, s->size, (int)(endp - p), p);
  return 0;
};

int scopeIdentifierSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  // printf("{%.*s}", (int)(endp - p - 1), p);
  return 0;
};

int varSizeSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  s->size = strtol((const char *)p, (char **)&endp, 10);
  return 0;
};

int idSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  // printf("{%.*s}", (int)(endp - p - 1), p);
  if (stringEq((s->trigger), p, endp)) {
    if (s->time < 10) {
      return 0;
    }
    if (s->start == 0) {
      s->start = s->time;
    } else {
      s->stop = s->time;
    }
  }
  return 0;
};

int vectorSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  // printf("{%.*s}", (int)(endp - p - 1), p);
  return 0;
};

int timeSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  s->time = strtol((const char *)p, (char **)&endp, 10);
  // printf("%d\n", s->time);
  return 0;
};
