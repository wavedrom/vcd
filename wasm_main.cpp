#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "vcd_parser.h"

using namespace std;


/// Typedef used as part of c->js call
typedef void externalJsMethodOne(const int sz);
typedef void externalJsMethodTwo(const char*, const uint64_t time, const uint8_t command, const int dnc0, const int dnc1);

typedef int  externalJsGetProperty(const char* name, const size_t len);
typedef void externalJsSetProperty(const char* name, const size_t len, const int type, const int v0, const int v1);



/// function pointer for c->js
static externalJsMethodOne* externalOne = 0;
static externalJsMethodTwo* externalTwo = 0;
static externalJsSetProperty* bound_set_property = 0;
static externalJsGetProperty* bound_get_property = 0;

void set_property_int(const char* name, const int value) {
  bound_set_property(name, strlen(name), 0, value, 0);
}

void set_property_string(const char* name, const char* value) {
  bound_set_property(name, strlen(name), 1, (int)value, strlen(value));
}

int get_property(const char* name) {
  return bound_get_property(name, strlen(name));
}


static struct vcd_parser_s* state;

extern "C" {

// returns context
int init(
  externalJsMethodOne* f1,
  externalJsMethodTwo* f2,
  externalJsSetProperty* sfn,
  externalJsGetProperty* gfn
  ) {
  

  state = (struct vcd_parser_s*) malloc(sizeof *state);

  const int32_t error = vcd_parser_init(state);
  if (error) {
    cout << "ERROR: " << error << "\n";
    return -1;
  }

  bound_set_property = sfn;
  bound_get_property = gfn;
  externalOne = f1;
  externalTwo = f2;

  state->lifee = (void*) externalOne;
  state->triee = (void*) externalTwo;

  static char triggerString [4096] = "       ";
  static char tmpStr [4096] = "       ";
  static uint64_t valueBuf [4096] = {};
  static uint64_t maskBuf [4096] = {};

  state->trigger = triggerString;
  state->reason = "NO REASON";
  state->napi_env = 0;
  state->tmpStr = tmpStr;
  state->value = valueBuf;
  state->mask = maskBuf;
  state->digitCount = 0;

  set_property_string("status", "declaration");


  static int context = 0;
  context++;

  return context;
}




int32_t execute(
  const int context,
  externalJsMethodOne* f1,
  externalJsMethodTwo* f2,
  externalJsSetProperty* sfn,
  externalJsGetProperty* gfn,
  char* p
  ) {

  // cout << "execute got " << p << "\n";
  cout << "execute " << (int)sfn << " and got " << p << "\n";
  bound_set_property = sfn;
  bound_get_property = gfn;
  externalOne = f1;
  externalTwo = f2;

  const size_t plen = strlen(p);

  const int32_t error = vcd_parser_execute(state, p, p + plen);

  return error;
}



// void execute(
//   const int context,
//   externalJsMethodOne* f1,
//   externalJsMethodTwo* f2,
//   externalJsSetProperty* sfn,
//   externalJsGetProperty* gfn,
//   char* chunk
//   ) {

//   // cout << "execute got " << p << "\n";
//   cout << "execute " << (int)sfn << " and got " << chunk << "\n";
//   bound_set_property = sfn;
//   bound_get_property = gfn;
//   externalOne = f1;
//   externalTwo = f2;

//   set_property_int("foo", 10);
  

//   int got = get_property("bar");

//   cout << "got " << got << " for bar\n";

// }

int main(void) {
  cout << "main()\n";
  return 0;
}


} // extern C