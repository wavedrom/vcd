#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include "vcd_parser.h"

using namespace std;


/// Typedef used as part of c->js call
typedef void externalJsMethodZero(const char* name, const size_t len);

// typedef void externalJsMethodOne (const char* name, const size_t len, const uint64_t time, const uint8_t command, const int valueWords, const int aValue, const int aMask);

typedef void externalJsMethodOne (
  const char* name,
  const size_t len,
  const int64_t time,
  const int command,
  const int valueWords,
  const int aValue,
  const int aMask
);

typedef int  externalJsGetProperty(const char* name, const size_t len);
typedef void externalJsSetProperty(const char* name, const size_t len, const int type, const int v0, const int v1);



/// function pointer for c->js
static externalJsMethodZero* externalZero = 0;
static externalJsMethodOne*  externalOne  = 0;
static externalJsSetProperty* bound_set_property = 0;
static externalJsGetProperty* bound_get_property = 0;
static struct vcd_parser_s* state;

extern "C" {

void set_property_int(const char* name, const int value) {
  bound_set_property(name, strlen(name), 0, value, 0);
}

void set_property_string(const char* name, const char* value) {
  bound_set_property(name, strlen(name), 1, (int)value, strlen(value));
}

void set_path_string(const char* name, const char* value) {
  bound_set_property(name, strlen(name), 2, (int)value, strlen(value));
}

void set_path_to_path(const char* name, const char* value) {
  bound_set_property(name, strlen(name), 3, (int)value, strlen(value));
}

void new_object_path(const char* name) {
  bound_set_property(name, strlen(name), 4, 0, 0);
}

int get_property_int(const char* name) {
  return bound_get_property(name, strlen(name));
}

void emit_lifee(const char* name) {
  externalZero(name, strlen(name));
}

void emit_triee(
  const char* name,
  const int64_t time,
  const int command,
  const int valueWords,
  uint64_t* aValue,
  uint64_t* aMask
) {

  // return;
  // externalOne(
  //   "hi"
  //   ,2
  //   ,time
  //   ,command
  //   ,0
  //   ,0
  //   ,0
  // );
  externalOne(
    name,
    strlen(name),
    time,
    command,
    valueWords,
    (int)aValue,
    (int)aMask
    );
}




// returns context
int init(
  externalJsMethodZero* f0,
  externalJsMethodOne*  f1,
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
  externalZero = f0;
  externalOne  = f1;

  state->lifee = 0;
  state->triee = 0;

  static char triggerString [4096] = {0};
  static char tmpStr [4096] = {0};
  static char tmpStr2 [4096] = {0};
  static char timeStampStr [4096] = {0};
  static char idStr [4096] = {0};
  static uint64_t valueBuf [4096] = {};
  static uint64_t maskBuf [4096] = {};

  state->trigger = triggerString;
  state->reason = "NO REASON";
  state->napi_env = 0;
  state->tmpStr = tmpStr;
  state->tmpStr2 = tmpStr2;
  state->timeStampStr = timeStampStr;
  state->idStr = idStr;
  state->value = valueBuf;
  state->mask = maskBuf;
  state->time = INT64_MAX;
  state->digitCount = 0;

  set_property_string("status", "declaration");


  static int context = 0;
  context++;

  return context;
}




int32_t execute(
  const int context,
  externalJsMethodZero* f0,
  externalJsMethodOne*  f1,
  externalJsSetProperty* sfn,
  externalJsGetProperty* gfn,
  char* p,
  const int plen
  ) {

  // cout << "execute got " << p << "\n";
  // cout << "execute " << (int)sfn << " and got " << p << "\n";
  bound_set_property = sfn;
  bound_get_property = gfn;
  externalZero = f0;
  externalOne  = f1;

  // const size_t plen = strlen(p);
  // printf("<chunk len|%d>\n", plen);


  const int32_t error = vcd_parser_execute(state, p, p + plen);

  return error;
}

int setTrigger(const int context, char* triggerString) {
  int triggerStringLen = strlen((char *)triggerString) - 1;
  // state->trigger = malloc(strlen(triggerString));
  char* p = (char *)state->tmpStr;
  for (int i = 0; i < triggerStringLen; i++) {
    char c = *(triggerString + i);
    if (c == 32) {
      c = 0;
    }
    *(p + i) = c;
  }
  // strcpy((char*)state->trigger, triggerString);
  // cout << "[" << triggerString << "|" << p << "\n";
  return 0;
}

int64_t getTime(const int context) {
  return state->time;
}



// void execute(
//   const int context,
//   externalJsMethodZero* f0,
//   externalJsMethodOne* f1,
//   externalJsSetProperty* sfn,
//   externalJsGetProperty* gfn,
//   char* chunk
//   ) {

//   // cout << "execute got " << p << "\n";
//   cout << "execute " << (int)sfn << " and got " << chunk << "\n";
//   bound_set_property = sfn;
//   bound_get_property = gfn;
//   externalZero = f0;
//   externalOne = f1;

//   set_property_int("foo", 10);


//   int got = get_property_int("bar");

//   cout << "got " << got << " for bar\n";

// }

int main(void) {
  // cout << "main()\n";
  return 0;
}


} // extern C
