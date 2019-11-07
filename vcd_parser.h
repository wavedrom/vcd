#ifndef INCLUDE_VCD_PARSER_H_
#define INCLUDE_VCD_PARSER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct vcd_parser_s vcd_parser_t;
struct vcd_parser_s {
  int32_t _index;
  void* _span_pos0;
  void* _span_cb0;
  void* _span_pos1;
  void* _span_cb1;
  int32_t error;
  const char* reason;
  const char* error_pos;
  void* data;
  void* _current;
  uint8_t command;
  uint8_t type;
  uint32_t size;
  uint32_t time;
  void* trigger;
  void* info;
  void* triee;
  void* lifee;
  void* hier;
  void* napi_env;
};

int vcd_parser_init(vcd_parser_t* s);
int vcd_parser_execute(vcd_parser_t* s, const char* p, const char* endp);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* INCLUDE_VCD_PARSER_H_ */
