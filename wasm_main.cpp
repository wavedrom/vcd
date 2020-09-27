#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "vcd_parser.h"

using namespace std;



static struct vcd_parser_s* state;


// returns context
int init(void) {
  state = (struct vcd_parser_s*) malloc(sizeof *state);

  const int32_t error = vcd_parser_init(state);
  if (error) {
    cout << "ERROR: " << error << "\n";
    return -1;
  }

  return 0;
}

int main(void) {
  cout << "main()\n";
  return 0;
}


/// Typedef used as part of c->js call
typedef void externalJsMethodOne(const int sz);
typedef void externalJsMethodTwo(const char*, const uint64_t time, const uint8_t command, const int dnc0, const int dnc1);


typedef int  externalJsGetProperty(const char* name);
typedef void externalJsSetProperty(const char* name, const size_t len, const int value);


/// function pointer for c->js
static externalJsMethodOne* externalOne = 0;
static externalJsMethodTwo* externalTwo = 0;
static externalJsSetProperty* set_property = 0;
static externalJsGetProperty* get_property = 0;



extern "C" {

void execute(
  externalJsMethodOne* f1,
  externalJsMethodTwo* f2,
  externalJsSetProperty* sfn,
  externalJsGetProperty* gfn
  ) {

  // cout << "execute got " << p << "\n";
  cout << "execute " << (int)sfn << "\n";
  set_property = sfn;

  set_property("foo", strlen("foo"), 4);
  

}

} // extern C