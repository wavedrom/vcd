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

METHOD(init) {
    struct vcd_parser_s *state = malloc(sizeof *state);

    const int32_t error = vcd_parser_init(state);

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
    ASSERT_STRING(args[1], p)
    ASSERT_STRING(args[2], endp)

    const int32_t error = vcd_parser_execute(state, p, endp);

    napi_value res;
    ASSERT(res, napi_create_int32(env, error, &res))
    return res;
}

napi_value Init(napi_env env, napi_value exports) {
    DECLARE_NAPI_METHOD("init", init)
    DECLARE_NAPI_METHOD("execute", execute)
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
