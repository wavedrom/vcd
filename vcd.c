#define NAPI_VERSION 6
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "vcd_parser.h"
#include <node_api.h>

#define DECLARE_NAPI_METHOD(name, func) { \
    napi_property_descriptor desc = { name, 0, func, 0, 0, 0, napi_default, 0 }; \
    if (napi_define_properties(env, exports, 1, &desc) != napi_ok) { \
        napi_throw_error(env, 0, "Error"); \
    } \
}

#define METHOD(name) napi_value name(napi_env env, napi_callback_info cbInfo)

#define ASSERT(val, expr) \
    if (expr != napi_ok) { \
        napi_throw(env, val); \
    }

#define ASSERT_ARGC(count) \
    napi_value args[count]; \
    { \
        size_t argc = count; \
        if (napi_get_cb_info(env, cbInfo, &argc, args, 0, 0) != napi_ok) { \
            napi_throw_error(env, 0, "Error"); \
            return 0; \
        } \
        if (argc < count) { \
            napi_throw_type_error(env, 0, "Wrong number of arguments"); \
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
  { \
    napi_value tmp; \
    if (napi_coerce_to_string(env, name, &tmp) != napi_ok) { \
      napi_throw(env, name); \
      return 0; \
    } \
    size_t result; \
    if (napi_get_value_string_latin1(env, tmp, var, 4096, &result) != napi_ok) { \
      napi_throw(env, name); \
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

#define ASSERT_FUNCTION(name, var) { \
    napi_valuetype valuetype; \
    if (napi_typeof(env, name, &valuetype) != napi_ok) { \
        napi_throw(env, name); \
        return 0; \
    } \
    if (valuetype != napi_function) { \
        napi_throw_type_error(env, 0, "Wrong arguments"); \
        return 0; \
    } \
    var = name; \
}

#define ASSERT_OBJECT(name, var) { \
    napi_valuetype valuetype; \
    if (napi_typeof(env, name, &valuetype) != napi_ok) { \
        napi_throw(env, name); \
        return 0; \
    } \
    if (valuetype != napi_object) { \
        napi_throw_type_error(env, 0, "Wrong arguments"); \
        return 0; \
    } \
    var = name; \
}




METHOD(init) {
  napi_value res;

  struct vcd_parser_s *state = malloc(sizeof *state);

  const int32_t error = vcd_parser_init(state);
  if (error) {
    ASSERT(res, napi_create_int32(env, error, &res))
    return res;
  }

  ASSERT_ARGC(3)
  ASSERT_FUNCTION(args[0], state->lifee)
  ASSERT_FUNCTION(args[1], state->triee)
  ASSERT_OBJECT(args[2], state->info)

  static char triggerString [4096] = "       ";
  static char tmpStr [4096] = "       ";
  static char tmpStr2 [4096] = "       ";
  static uint64_t valueBuf [4096] = {};
  static uint64_t maskBuf [4096] = {};

  state->trigger = triggerString;
  state->reason = "NO REASON";
  state->napi_env = env;
  state->tmpStr = tmpStr;
  state->tmpStr2 = tmpStr2;
  state->value = valueBuf;
  state->mask = maskBuf;
  state->time = UINT64_MAX;
  state->digitCount = 0;

  napi_value status;
  ASSERT(status, napi_create_string_latin1(env, "declaration", NAPI_AUTO_LENGTH, &status))
  ASSERT(state->info, napi_set_named_property(env, state->info, "status", status))
  // ASSERT(status, napi_get_named_property(env, state->info, "wires", &state->wires))

  // napi_value hierObj;
  // ASSERT(hierObj, napi_create_object(env, &hierObj))
  // state->info = hierObj;

  // ASSERT(state->info, napi_create_object(env, &state->info))

  ASSERT(res, napi_create_external(env, state, 0, 0, &res))
  return res;
}

METHOD(done) {
  ASSERT_ARGC(4)
  struct vcd_parser_s *state;
  // last use of all external objects
  napi_value lifee, triee, info;
  ASSERT_EXTERNAL(args[0], state)
  ASSERT_FUNCTION(args[1], lifee)
  ASSERT_FUNCTION(args[2], triee)
  ASSERT_OBJECT(args[3], info)

  // FIXME destroy parser state

  const int32_t error = 0;
  napi_value res;
  ASSERT(res, napi_create_int32(env, error, &res))
  return res;
}

METHOD(execute) {
  ASSERT_ARGC(5)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)
  ASSERT_FUNCTION(args[1], state->lifee)
  ASSERT_FUNCTION(args[2], state->triee)
  ASSERT_OBJECT(args[3], state->info)
  ASSERT_BUFFER(args[4], p, plen)

  state->napi_env = env;

  const int32_t error = vcd_parser_execute(state, p, p + plen);
  napi_value res;
  ASSERT(res, napi_create_int32(env, error, &res))
  return res;
}

METHOD(setTrigger) {
  ASSERT_ARGC(2)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)
  ASSERT_STRING(args[1], state->trigger)

  napi_value res;
  ASSERT(res, napi_create_int32(env, state->error, &res))
  return res;
}

METHOD(getTime) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value res;
  ASSERT(res, napi_create_bigint_uint64(env, state->time, &res))
  return res;
}

napi_value Init(napi_env env, napi_value exports) {
  DECLARE_NAPI_METHOD("init", init)
  DECLARE_NAPI_METHOD("done", done)
  DECLARE_NAPI_METHOD("execute", execute)
  DECLARE_NAPI_METHOD("setTrigger", setTrigger)
  DECLARE_NAPI_METHOD("getTime", getTime)
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
