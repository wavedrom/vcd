#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "vcd_parser.h"

#ifndef VCDWASM
#include <node_api.h>
#else
#include "wasm_main.hpp"
#endif

#ifdef VCDWASM
typedef void* napi_env;
#endif


// #define LOGSPAN
// #define LOGSPAN printf("%s\n", __FUNCTION__);


#define ASSERT(val, expr) \
    if (expr != napi_ok) { \
        napi_throw(env, val); \
    }

void strcopy(const unsigned char* p, const unsigned char* endp, unsigned char* dst) {
  const unsigned char* src;
  src = p;
  while (src < (endp - 1)) {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = 0;
}

void strconcat(const unsigned char* p, const unsigned char* endp, unsigned char* dst) {
  // printf("<len:%d>", endp - p);
  dst += strlen((char *)dst); // go to the end of string
  while (p < endp) {
    *dst = *p;
    p++;
    dst++;
  }
  *dst = 0;
}

// FIXME use a better structure to match strings
int stringEq (
  const unsigned char* i, // search pattern
  const unsigned char* p,
  const unsigned char* endp
) {
  if (*i == 0) {
    return 1;
  }
  const unsigned char* j;
  while (1) {
    j = p;
    // printf("(%s|%s)", (char *)i, (char *)j);
    if (*i == 0) { // end of search pattern
      return 0;
    }
    while (*i == *j) { // follow matching trail
      if (*i == 0 && *j == 0) { // match zeros
        return 1;
      }
      i++;
      j++;
    }
    while (*i != 0) { // skip to the end of pattern word
      i++;
    }
    i++;
  }
}

int commandSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {

#ifndef VCDWASM
  napi_env env = state->napi_env;
#endif

  if (state->command == 5) { // $upscope
    state->stackPointer -= 1;
    return 0;
  }

  if (
    (state->command == 1) || // $comment
    (state->command == 2) || // $date
    (state->command == 4) || // $timescale
    (state->command == 7)    // $version
  ) {
    char* key;
    switch(state->command) {
    case 1: key = "comment"; break;
    case 2: key = "date"; break;
    case 4: key = "timescale"; break;
    case 7: key = "version"; break;
    }
#ifndef VCDWASM
    napi_value val;
    ASSERT(val, napi_create_string_latin1(env, (char*)p, (endp - p - 4), &val))
    ASSERT(state->info, napi_set_named_property(env, state->info, key, val))
#else
    strcopy(p, endp - 3, state->tmpStr);
    set_property_string(key, state->tmpStr);
#endif
    return 0;
  }

  if (state->command == 8) { // $enddefinitions
    *(char *)state->idStr = 0;
    *(char *)state->timeStampStr = 0;
#ifndef VCDWASM
    napi_value status, undefined, eventName, eventPayload, return_val;
    ASSERT(status, napi_create_string_latin1(env, "simulation", NAPI_AUTO_LENGTH, &status))
    ASSERT(state->info, napi_set_named_property(env, state->info, "status", status))
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, "$enddefinitions", NAPI_AUTO_LENGTH, &eventName))
    // ASSERT(eventPayload, napi_create_string_latin1(env, "payload", NAPI_AUTO_LENGTH, &eventPayload))
    napi_value* argv[] = { &eventName }; // , &eventPayload };
    ASSERT(state->lifee, napi_call_function(env, undefined, state->lifee, 1, *argv, &return_val))
#else
    set_property_string("status", "simulation");
    emit_lifee("$enddefinitions");
#endif
    return 0;
  }

  return 0;
}

int scopeIdentifierSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {

  strcopy(p, endp, state->tmpStr); // load the value into temp string 1
#ifndef VCDWASM
  napi_env env = state->napi_env;
  napi_value obj, stack, top;
  ASSERT(obj, napi_create_object(env, &obj))
  ASSERT(state->info, napi_get_named_property(env, state->info, "stack", &stack))

  // get the top of the stack in top
  ASSERT(top, napi_get_element(env, stack, state->stackPointer, &top))

  // set top.prop to new object
  ASSERT(top, napi_set_named_property(env, top, state->tmpStr, obj))

  state->stackPointer += 1;
  ASSERT(top, napi_set_element(env, stack, state->stackPointer, obj))
#else
  // set stack[sp].`tmpStr` to {}
  snprintf(state->tmpStr2, 4096, "stack.%d.%s", state->stackPointer, state->tmpStr);
  new_object_path(state->tmpStr2);

  // bump
  state->stackPointer += 1;

  // set stack[sp+1] to the same object as stack[sp].`tmpStr`
  snprintf(state->tmpStr, 4096, "stack.%d", state->stackPointer);

  set_path_to_path(state->tmpStr, state->tmpStr2);
#endif
  return 0;
}

int varSizeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  state->size = strtol((const char *)p, (char **)&endp, 10);
  return 0;
}

int varIdSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
#ifndef VCDWASM
  napi_env env = state->napi_env;
  napi_value varId;
  ASSERT(varId, napi_create_string_latin1(env, (char*)p, (endp - p - 1), &varId))
  ASSERT(state->info, napi_set_named_property(env, state->info, "varId", varId))
#else
  strcopy(p, endp, state->tmpStr);
  set_property_string("varId", state->tmpStr);
#endif
  return 0;
}

int varNameSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
#ifndef VCDWASM
  napi_env env = state->napi_env;
  // *(endp - 1) = 0; // FIXME NULL termination of ASCII string
  strcopy(p, endp, state->tmpStr);
  napi_value stack, top, varId;
  ASSERT(state->info, napi_get_named_property(env, state->info, "stack", &stack))
  ASSERT(top, napi_get_element(env, stack, state->stackPointer, &top))
  ASSERT(state->info, napi_get_named_property(env, state->info, "varId", &varId))
  ASSERT(state->info, napi_set_named_property(env, top, state->tmpStr, varId))
#else
  strcopy(p, endp, state->tmpStr);
  // set
  //  info.stack[sp].`tmpStr` = info.varId
  snprintf(state->tmpStr2, 4096, "stack.%d.%s", state->stackPointer, state->tmpStr);
  set_path_to_path(state->tmpStr2, "varId");
#endif
  return 0;
}

int idSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  strconcat(p, endp, state->idStr);
  // printf("<idSpan|%s>\n", (char *)state->idStr);
  return 0;
}

int onId (vcd_parser_t* state, const unsigned char* _p, const unsigned char* _endp) {
#ifndef VCDWASM
  napi_env env = state->napi_env;
#endif
  const unsigned char* p = (char *)state->idStr;
  const unsigned int plen = strlen((char *)p) - 1;
  *(char *)(p + plen) = 0; // null instead of space
  const unsigned char* endp = p + plen - 1;
  // printf("<onId|%s>\n", (char *)state->idStr);

  const int valueWords = (state->digitCount >> 6) + 1;
  uint64_t* value = state->value;
  uint64_t* mask = state->mask;
  if (stringEq((state->trigger), p, endp)) {
    const uint8_t command = state->command;
    // printf("{id:'%s',cmd:%d}", (char *)p, command);
    if (command == 14) {
      value[0] = 0;
      mask[0] = 0;
    } else
    if (command == 15) {
      value[0] = 1;
      mask[0] = 0;
    }
#ifndef VCDWASM
    napi_value undefined, eventName, aTime, aCommand, aValue, aMask, return_val;
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, (char*)p, NAPI_AUTO_LENGTH, &eventName))
    ASSERT(aTime, napi_create_int64(env, state->time, &aTime))
    ASSERT(aCommand, napi_create_int32(env, command, &aCommand))
    ASSERT(aValue, napi_create_bigint_words(env, 0, valueWords, value, &aValue))
    ASSERT(aMask, napi_create_bigint_words(env, 0, valueWords, mask, &aMask))
    napi_value* argv[] = {&eventName, &aTime, &aCommand, &aValue, &aMask};
    ASSERT(state->triee, napi_call_function(env, undefined, state->triee, 5, *argv, &return_val))
    // printf("<id='%s'>", (char *)p);
#else
    // strcopy(p, endp, state->tmpStr);
    emit_triee((char *)p, state->time, command, valueWords, value, mask);
#endif
  }
  for (int i = 0; i < valueWords; i++) {
    value[i] = 0;
    mask[i] = 0;
  }
  state->digitCount = 0;
  *(char *)state->idStr = 0;
  return 0;
}



int onDigit(
  vcd_parser_t* state,
  const unsigned char* p,
  const unsigned char* endp,
  int digit
) {
  unsigned int valueCin = (digit & 1);
  unsigned int maskCin = ((digit >> 1) & 1);
  unsigned int valueCout;
  unsigned int maskCout;
  uint64_t* value = state->value;
  uint64_t* mask = state->mask;
  const int valueWordsMinus = (state->digitCount >> 6);
  for (int i = 0; i <= valueWordsMinus; i++) {

    valueCout = value[i] >> 63;
    value[i] = (value[i] << 1) + valueCin;
    valueCin = valueCout;

    maskCout = mask[i] >> 63;
    mask[i]  = (mask[i] << 1) + maskCin;
    maskCin = maskCout;

  }
  state->digitCount += 1;
  return 0;
}

int onRecover(
  vcd_parser_t* state,
  const unsigned char* p,
  const unsigned char* endp,
  int digit
) {
  state->digitCount = 0;
  return 0;
}


int timeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  strconcat(p, endp, state->timeStampStr);
  // printf("<timeSpan|%s>\n", (char *)state->timeStampStr);
  return 0;
}

int onTime (vcd_parser_t* state, const unsigned char* _p, const unsigned char* _endp) {
  char *end;
  const int64_t time = strtoul(state->timeStampStr, &end, 10);
  // printf("<onTime|%lu>\n", time);
  if (state->time == INT64_MAX) {
#ifndef VCDWASM
    napi_env env = state->napi_env;
    napi_value val;
    ASSERT(val, napi_create_int64(env, time, &val))
    ASSERT(state->info, napi_set_named_property(env, state->info, "t0", val))
#else
    set_property_int("t0", time);
#endif
  }
  state->time = time;
  *(char *)state->timeStampStr = 0;
  return 0;
}
