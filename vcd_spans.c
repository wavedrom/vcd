#include <stdio.h>
#include <stdlib.h>
#include "vcd_parser.h"
#include <node_api.h>

#define ASSERT(val, expr) \
    if (expr != napi_ok) { \
        napi_throw(env, val); \
    }

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

int commandSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  napi_env env = state->napi_env;

  if (state->command == 5) {
    // printf(")");
    return 0;
  }

  if (state->command == 8) { // $enddefinitions
    napi_value status;
    ASSERT(status, napi_create_string_latin1(env, "simulation", NAPI_AUTO_LENGTH, &status))
    ASSERT(state->hier, napi_set_named_property(env, state->hier, "status", status))

    napi_value undefined, eventName, eventPayload, return_val;
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, "$enddefinitions", NAPI_AUTO_LENGTH, &eventName))
    // ASSERT(eventPayload, napi_create_string_latin1(env, "payload", NAPI_AUTO_LENGTH, &eventPayload))
    napi_value* argv[] = { &eventName }; // , &eventPayload };
    ASSERT(state->lifee, napi_call_function(env, undefined, state->lifee, 1, *argv, &return_val))
    return 0;
  }

  return 0;
}

int scopeIdentifierSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  // printf("(%d:%d", state->type, state->size);
  return 0;
}

int varSizeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  state->size = strtol((const char *)p, (char **)&endp, 10);
  return 0;
}

int varIdSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  // printf(" %d", state->type);
  return 0;
}

int idSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  napi_env env = state->napi_env;
  if (stringEq((state->trigger), p, endp)) {
    napi_value undefined, eventName, eventPayload1, eventPayload2, return_val;
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, (char*)p, (endp - p - 1), &eventName))
    ASSERT(eventPayload1, napi_create_int32(env, state->time, &eventPayload1))
    ASSERT(eventPayload2, napi_create_int32(env, state->command, &eventPayload2))
    napi_value* argv[] = { &eventName, &eventPayload1, &eventPayload2 };
    ASSERT(state->triee, napi_call_function(env, undefined, state->triee, 3, *argv, &return_val))
    // printf("{%.*s}", (int)(endp - p - 1), p);
  }
  return 0;
}

int vectorSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  // printf("{%.*s}", (int)(endp - p - 1), p);
  return 0;
}

int timeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  state->time = strtol((const char *)p, (char **)&endp, 10);
  return 0;
}
