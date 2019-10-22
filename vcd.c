#include <stdio.h>
#include <stdlib.h>
#include "vcd_parser.h"
#include <node_api.h>

#define DECLARE_NAPI_METHOD(name, func) { \
    napi_property_descriptor desc = { name, 0, func, 0, 0, 0, napi_default, 0 }; \
    if (napi_define_properties(env, exports, 1, &desc) != napi_ok) { \
        napi_throw_error(env, 0, "Error"); \
    } \
}

#define METHOD(name) \
    napi_value name(napi_env env, napi_callback_info info)

#define ASSERT(val, expr) \
    if (expr != napi_ok) { \
        napi_throw(env, val); \
    }

#define ASSERT_ARGC(count) \
    napi_value args[count]; \
    { \
        size_t argc = count; \
        if (napi_get_cb_info(env, info, &argc, args, 0, 0) != napi_ok) { \
            napi_throw_error(env, 0, "Error"); \
            return 0; \
        } \
        if (argc < count) { \
            napi_throw_type_error(env, 0, "Wrong number of arguments"); \
            return 0; \
        } \
    }

#define ASSERT_EXTERNAL(name, var) { \
    napi_valuetype valuetype; \
    if (napi_typeof(env, name, &valuetype) != napi_ok) { \
        napi_throw(env, name); \
        return 0; \
    } \
    if (valuetype != napi_external) { \
        napi_throw_type_error(env, 0, "Wrong arguments"); \
        return 0; \
    } \
    if (napi_get_value_external(env, name, (void **)(&var)) != napi_ok) { \
        napi_throw(env, name); \
        return 0; \
    } \
}

#define ASSERT_BUFFER(name, var, len) \
    const char * var; \
    size_t len; \
    { \
        bool result; \
        if(napi_is_buffer(env, name, &result) != napi_ok) { \
            napi_throw(env, name); \
            return 0; \
        } \
        if (napi_get_buffer_info(env, name, (void **)&var, &len) != napi_ok) { \
            napi_throw(env, name); \
            return 0; \
        } \
    }

#define ASSERT_STRING(name, var) \
    char var[256]; \
    { \
        napi_value tmp; \
        if (napi_coerce_to_string(env, name, &tmp) != napi_ok) { \
            napi_throw(env, name); \
            return 0; \
        } \
        size_t result; \
        if (napi_get_value_string_latin1(env, tmp, var, 256, &result) != napi_ok) { \
            napi_throw(env, name); \
            return 0; \
        } \
    }

METHOD(init) {
    struct vcd_parser_s *state = malloc(sizeof *state);

    const int32_t error = vcd_parser_init(state);
    state->trigger = "HELLO";

    napi_value res;
    if (error) {
        ASSERT(res, napi_create_int32(env, error, &res))
    } else {
        ASSERT(res, napi_create_external(env, state, 0, 0, &res))
    }
    return res;
}

METHOD(execute) {
    ASSERT_ARGC(2)
    struct vcd_parser_s *state;
    ASSERT_EXTERNAL(args[0], state)
    ASSERT_BUFFER(args[1], p, plen)

    const int32_t error = vcd_parser_execute(state, p, p + plen);

    napi_value res;
    ASSERT(res, napi_create_int32(env, error, &res))
    return res;
}

METHOD(getError) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->error, &res))
  return res;
}

METHOD(getReason) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_string_utf8(env, state->reason, NAPI_AUTO_LENGTH, &res))
  return res;
}

METHOD(getErrorPos) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->error_pos, &res))
  return res;
}

METHOD(getCommand) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->command, &res))
  return res;
}

METHOD(getTime) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->time, &res))
  return res;
}

METHOD(getStart) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->start, &res))
  return res;
}

METHOD(getStop) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->stop, &res))
  return res;
}

METHOD(setTrigger) {
  ASSERT_ARGC(2)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)
  ASSERT_STRING(args[1], trigger)

  state->trigger = *trigger;

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->error, &res))
  return res;
}

napi_value Init(napi_env env, napi_value exports) {
  DECLARE_NAPI_METHOD("init", init)
  DECLARE_NAPI_METHOD("execute", execute)
  DECLARE_NAPI_METHOD("getError", getError)
  DECLARE_NAPI_METHOD("getReason", getReason)
  DECLARE_NAPI_METHOD("getErrorPos", getErrorPos)
  DECLARE_NAPI_METHOD("getCommand", getCommand)
  DECLARE_NAPI_METHOD("getTime", getTime)
  DECLARE_NAPI_METHOD("getStart", getStart)
  DECLARE_NAPI_METHOD("getStop", getStop)
  DECLARE_NAPI_METHOD("setTrigger", setTrigger)
  return exports;
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
  s->size = strtol(p, &endp, 10);
  return 0;
};

bool stringEq (
  const unsigned char* gold,
  const unsigned char* p,
  const unsigned char* endp
) {
  for (size_t i = 0; gold[i] != 0; i++) {
    if (gold[i] != p[i]) {
      return false;
    }
  }
  return true;
}

int idSpan(vcd_parser_t* s, const unsigned char* p, const unsigned char* endp) {
  // printf("{%.*s}", (int)(endp - p - 1), p);
  if (stringEq("D1", p, endp)) {
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
  s->time = strtol(p, &endp, 10);
  // printf("%d\n", s->time);
  return 0;
};

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
