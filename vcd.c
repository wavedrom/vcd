#define NAPI_VERSION 1
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

#define METHOD(name) napi_value name(napi_env env, napi_callback_info info)

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
    if (napi_get_value_string_latin1(env, tmp, var, 256, &result) != napi_ok) { \
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

// napi_value * var;


METHOD(init) {
  struct vcd_parser_s *state = malloc(sizeof *state);

  const int32_t error = vcd_parser_init(state);

  static char triggerString [256];

  state->trigger = triggerString;
  state->reason = "NO REASON";

  napi_value res;
  if (error) {
    ASSERT(res, napi_create_int32(env, error, &res))
  } else {
    ASSERT(res, napi_create_external(env, state, 0, 0, &res))
  }
  return res;
}

METHOD(execute) {
  ASSERT_ARGC(3)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)
  ASSERT_FUNCTION(args[1], state->emit)
  ASSERT_BUFFER(args[2], p, plen)

  state->napi_env = env;

  const int32_t error = vcd_parser_execute(state, p, p + plen);

  napi_value res;
  ASSERT(res, napi_create_int32(env, error, &res))
  return res;
}

METHOD(getInfo) {
  ASSERT_ARGC(1)
  struct vcd_parser_s *state;
  ASSERT_EXTERNAL(args[0], state)

  napi_value infObj, error, reason, command, type, size, time, trigger;
  ASSERT(infObj, napi_create_object(env, &infObj))

  ASSERT(error, napi_create_int32(env, state->error, &error))
  ASSERT(infObj, napi_set_named_property(env, infObj, "error", error))

  ASSERT(reason, napi_create_string_latin1(env, state->reason, NAPI_AUTO_LENGTH, &reason))
  ASSERT(infObj, napi_set_named_property(env, infObj, "reason", reason))

  ASSERT(command, napi_create_int32(env, state->command, &command))
  ASSERT(infObj, napi_set_named_property(env, infObj, "command", command))

  ASSERT(type, napi_create_int32(env, state->type, &type))
  ASSERT(infObj, napi_set_named_property(env, infObj, "type", type))

  ASSERT(size, napi_create_int32(env, state->size, &size))
  ASSERT(infObj, napi_set_named_property(env, infObj, "size", size))

  ASSERT(time, napi_create_int32(env, state->time, &time))
  ASSERT(infObj, napi_set_named_property(env, infObj, "time", time))

  ASSERT(trigger, napi_create_string_latin1(env, state->trigger, NAPI_AUTO_LENGTH, &trigger))
  ASSERT(infObj, napi_set_named_property(env, infObj, "trigger", trigger))

  return infObj;
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

napi_value Init(napi_env env, napi_value exports) {
  DECLARE_NAPI_METHOD("init", init)
  DECLARE_NAPI_METHOD("execute", execute)
  DECLARE_NAPI_METHOD("getInfo", getInfo)
  DECLARE_NAPI_METHOD("setTrigger", setTrigger)
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
