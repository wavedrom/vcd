#include <stdio.h>
#include <stdlib.h>
#include "vcd_parser.h"
#include <node_api.h>

int stringEq (
  const unsigned char* gold,
  const unsigned char* p,
  const unsigned char* endp
) {
  if (gold[0] == 0) {
    return 0;
  }
  for (size_t i = 0; gold[i] != 0; i++) {
    if (gold[i] != p[i]) {
      return 0;
    }
  }
  return 1;
}

int commandSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  return 0;
};

int scopeIdentifierSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  return 0;
};

int varSizeSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  s->size = strtol((const char *)p, (char **)&endp, 10);
  return 0;
};

int idSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  if (stringEq((state->trigger), p, endp)) {
    napi_value undefined;
    if (napi_get_undefined(state->napi_env, &undefined) != napi_ok) {
      napi_throw(state->napi_env, undefined);
      return 0;
    }

    napi_value eventName;
    if (napi_create_string_latin1(state->napi_env, "trigger", NAPI_AUTO_LENGTH, &eventName) != napi_ok) {
      napi_throw(state->napi_env, eventName);
      return 0;
    };

    napi_value eventPayload;
    if (napi_create_int32(state->napi_env, state->time, &eventPayload) != napi_ok) {
      napi_throw(state->napi_env, eventPayload);
      return 0;
    };

    napi_value* argv[] = { &eventName, &eventPayload };

    napi_value return_val;
    if (napi_call_function(state->napi_env, undefined, state->emit, 2, *argv, &return_val) != napi_ok) {
      napi_throw(state->napi_env, state->emit);
      return 0;
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
  return 0;
};
