#pragma once
#include <stdint.h>

void set_property_int(const char* name, const int value);
void set_property_string(const char* name, const char* value);
void set_path_string(const char* name, const char* value);
void set_path_to_path(const char* name, const char* value);
void new_object_path(const char* name);
void on_command(const char* body, const int command);
int get_property_int(const char* name);
void emit_lifee(const char* name);
void emit_triee(
  const char* name,
  const int64_t time,
  const int command,
  const int valueWords,
  uint64_t* aValue,
  const int maskWords,
  uint64_t* aMask
);
