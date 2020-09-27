#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "vcd_parser.h"

using namespace std;



static struct vcd_parser_s* state;


static void init(void) {
  state = (struct vcd_parser_s*) malloc(sizeof *state);

  const int32_t error = vcd_parser_init(state);
  if (error) {
    cout << "ERROR: " << error << "\n";
    return;
  }
}

int main(void) {
  cout << "main()\n";
  init();
  return 0;
}

