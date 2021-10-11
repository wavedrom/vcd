#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __SSE4_2__
 #ifdef _MSC_VER
  #include <nmmintrin.h>
 #else  /* !_MSC_VER */
  #include <x86intrin.h>
 #endif  /* _MSC_VER */
#endif  /* __SSE4_2__ */

#ifdef _MSC_VER
 #define ALIGN(n) _declspec(align(n))
#else  /* !_MSC_VER */
 #define ALIGN(n) __attribute__((aligned(n)))
#endif  /* _MSC_VER */

#include "vcd_parser.h"

typedef int (*vcd_parser__span_cb)(
             vcd_parser_t*, const char*, const char*);

static const unsigned char llparse_blob0[] = {
  'o', 'm', 'm', 'e', 'n', 't'
};
static const unsigned char llparse_blob1[] = {
  '$', 'e', 'n', 'd'
};
static const unsigned char llparse_blob2[] = {
  'a', 't', 'e'
};
static const unsigned char llparse_blob3[] = {
  'n', 'd', 'd', 'e', 'f', 'i', 'n', 'i', 't', 'i', 'o',
  'n', 's'
};
static const unsigned char llparse_blob4[] = {
  '$', 'e', 'n', 'd'
};
static const unsigned char llparse_blob5[] = {
  'o', 'm', 'm', 'e', 'n', 't'
};
static const unsigned char llparse_blob6[] = {
  '$', 'e', 'n', 'd'
};
static const unsigned char llparse_blob7[] = {
  'u', 'm', 'p'
};
static const unsigned char llparse_blob8[] = {
  'l', 'l'
};
static const unsigned char llparse_blob9[] = {
  'a', 'r', 's'
};
static const unsigned char llparse_blob10[] = {
  'n', 'd'
};
static const unsigned char llparse_blob11[] = {
  'c', 'o', 'p', 'e'
};
static const unsigned char llparse_blob12[] = {
  'e', 'g', 'i', 'n'
};
static const unsigned char llparse_blob13[] = {
  'r', 'k'
};
static const unsigned char llparse_blob14[] = {
  'n', 'c', 't', 'i', 'o', 'n'
};
static const unsigned char llparse_blob15[] = {
  'o', 'd', 'u', 'l', 'e'
};
static const unsigned char llparse_blob16[] = {
  'a', 's', 'k'
};
static const unsigned char llparse_blob17[] = {
  'i', 'm', 'e', 's', 'c', 'a', 'l', 'e'
};
static const unsigned char llparse_blob18[] = {
  'p', 's', 'c', 'o', 'p', 'e'
};
static const unsigned char llparse_blob19[] = {
  'v', 'e', 'n', 't'
};
static const unsigned char llparse_blob20[] = {
  'n', 't', 'e', 'g', 'e', 'r'
};
static const unsigned char llparse_blob21[] = {
  'a', 'r', 'a', 'm', 'e', 't', 'e', 'r'
};
static const unsigned char llparse_blob22[] = {
  'i', 'm', 'e'
};
static const unsigned char llparse_blob23[] = {
  'u', 'p', 'p', 'l', 'y'
};
static const unsigned char llparse_blob24[] = {
  'm', 'e'
};
static const unsigned char llparse_blob25[] = {
  'n', 'd'
};
static const unsigned char llparse_blob26[] = {
  'e', 'g'
};
static const unsigned char llparse_blob27[] = {
  'n', 'd'
};
static const unsigned char llparse_blob28[] = {
  'r', 'e'
};
static const unsigned char llparse_blob29[] = {
  'r', 's', 'i', 'o', 'n'
};

enum llparse_match_status_e {
  kMatchComplete,
  kMatchPause,
  kMatchMismatch
};
typedef enum llparse_match_status_e llparse_match_status_t;

struct llparse_match_s {
  llparse_match_status_t status;
  const unsigned char* current;
};
typedef struct llparse_match_s llparse_match_t;

static llparse_match_t llparse__match_sequence_id(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp,
    const unsigned char* seq, uint32_t seq_len) {
  uint32_t index;
  llparse_match_t res;

  index = s->_index;
  for (; p != endp; p++) {
    unsigned char current;

    current = *p;
    if (current == seq[index]) {
      if (++index == seq_len) {
        res.status = kMatchComplete;
        goto reset;
      }
    } else {
      res.status = kMatchMismatch;
      goto reset;
    }
  }
  s->_index = index;
  res.status = kMatchPause;
  res.current = p;
  return res;
reset:
  s->_index = 0;
  res.current = p;
  return res;
}

enum llparse_state_e {
  s_error,
  s_n_vcd_parser__n_inDeclaration,
  s_n_vcd_parser__n_span_start_commandSpan,
  s_n_vcd_parser__n_declaration_2,
  s_n_vcd_parser__n_declaration_3,
  s_n_vcd_parser__n_simulationTime,
  s_n_vcd_parser__n_span_start_timeSpan,
  s_n_vcd_parser__n_inSimulation,
  s_n_vcd_parser__n_span_start_commandSpan_2,
  s_n_vcd_parser__n_simulation_2,
  s_n_vcd_parser__n_simulation_5,
  s_n_vcd_parser__n_simulation_7,
  s_n_vcd_parser__n_simulation_6,
  s_n_vcd_parser__n_simulation_8,
  s_n_vcd_parser__n_simulation_4,
  s_n_vcd_parser__n_simulation_3,
  s_n_vcd_parser__n_simulation_9,
  s_n_vcd_parser__n_simulation_1,
  s_n_vcd_parser__n_simulationId,
  s_n_vcd_parser__n_span_start_idSpan,
  s_n_vcd_parser__n_simulationVectorEnd,
  s_n_vcd_parser__n_simulationVector,
  s_n_vcd_parser__n_simulation,
  s_n_vcd_parser__n_inDeclarationEnd,
  s_n_vcd_parser__n_span_start_commandSpan_1,
  s_n_vcd_parser__n_declaration_4,
  s_n_vcd_parser__n_scopeIdentifierEnd,
  s_n_vcd_parser__n_span_start_scopeIdentifierSpan,
  s_n_vcd_parser__n_scopeIdentifier,
  s_n_vcd_parser__n_scopeTypeEnd_1,
  s_n_vcd_parser__n_scopeTypeEnd_3,
  s_n_vcd_parser__n_scopeTypeEnd_4,
  s_n_vcd_parser__n_scopeTypeEnd_2,
  s_n_vcd_parser__n_scopeTypeEnd_5,
  s_n_vcd_parser__n_scopeTypeEnd_6,
  s_n_vcd_parser__n_scopeTypeEnd,
  s_n_vcd_parser__n_scopeType,
  s_n_vcd_parser__n_span_start_commandSpan_3,
  s_n_vcd_parser__n_declaration_5,
  s_n_vcd_parser__n_declaration_6,
  s_n_vcd_parser__n_declaration_7,
  s_n_vcd_parser__n_varNameEnd,
  s_n_vcd_parser__n_span_start_varNameSpan,
  s_n_vcd_parser__n_varName,
  s_n_vcd_parser__n_varIdEnd,
  s_n_vcd_parser__n_span_start_varIdSpan,
  s_n_vcd_parser__n_varId,
  s_n_vcd_parser__n_varSizeEnd,
  s_n_vcd_parser__n_span_start_varSizeSpan,
  s_n_vcd_parser__n_varSize,
  s_n_vcd_parser__n_varTypeEnd_1,
  s_n_vcd_parser__n_varTypeEnd_2,
  s_n_vcd_parser__n_varTypeEnd_3,
  s_n_vcd_parser__n_varTypeEnd_8,
  s_n_vcd_parser__n_varTypeEnd_7,
  s_n_vcd_parser__n_varTypeEnd_6,
  s_n_vcd_parser__n_varTypeEnd_5,
  s_n_vcd_parser__n_varTypeEnd_4,
  s_n_vcd_parser__n_varTypeEnd_10,
  s_n_vcd_parser__n_varTypeEnd_9,
  s_n_vcd_parser__n_varTypeEnd_12,
  s_n_vcd_parser__n_varTypeEnd_15,
  s_n_vcd_parser__n_varTypeEnd_16,
  s_n_vcd_parser__n_varTypeEnd_17,
  s_n_vcd_parser__n_varTypeEnd_14,
  s_n_vcd_parser__n_varTypeEnd_13,
  s_n_vcd_parser__n_varTypeEnd_11,
  s_n_vcd_parser__n_varTypeEnd_19,
  s_n_vcd_parser__n_varTypeEnd_20,
  s_n_vcd_parser__n_varTypeEnd_21,
  s_n_vcd_parser__n_varTypeEnd_18,
  s_n_vcd_parser__n_varTypeEnd,
  s_n_vcd_parser__n_varType,
  s_n_vcd_parser__n_span_start_commandSpan_4,
  s_n_vcd_parser__n_declaration_9,
  s_n_vcd_parser__n_declaration_10,
  s_n_vcd_parser__n_declaration_8,
  s_n_vcd_parser__n_declaration_1,
  s_n_vcd_parser__n_declaration,
};
typedef enum llparse_state_e llparse_state_t;

int commandSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int idSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int timeSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int varSizeSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int varIdSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int varNameSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int scopeIdentifierSpan(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp);

int vcd_parser__c_store_command(
    vcd_parser_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->command = match;
  return 0;
}

int onDigit(
    vcd_parser_t* s, const unsigned char* p,
    const unsigned char* endp,
    int value);

int vcd_parser__c_store_type(
    vcd_parser_t* state,
    const unsigned char* p,
    const unsigned char* endp,
    int match) {
  state->type = match;
  return 0;
}

int vcd_parser_init(vcd_parser_t* state) {
  memset(state, 0, sizeof(*state));
  state->_current = (void*) (intptr_t) s_n_vcd_parser__n_declaration;
  return 0;
}

static llparse_state_t vcd_parser__run(
    vcd_parser_t* state,
    const unsigned char* p,
    const unsigned char* endp) {
  int match;
  switch ((llparse_state_t) (intptr_t) state->_current) {
    case s_n_vcd_parser__n_inDeclaration:
    s_n_vcd_parser__n_inDeclaration: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_inDeclaration;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob1, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_span_end_commandSpan;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_inDeclaration;
        }
        case kMatchMismatch: {
          p++;
          goto s_n_vcd_parser__n_inDeclaration;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_commandSpan:
    s_n_vcd_parser__n_span_start_commandSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_commandSpan;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = commandSpan;
      goto s_n_vcd_parser__n_inDeclaration;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_2:
    s_n_vcd_parser__n_declaration_2: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob0, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_vcd_parser__n_invoke_store_command;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_2;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_3:
    s_n_vcd_parser__n_declaration_3: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_3;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob2, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_vcd_parser__n_invoke_store_command;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_3;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulationTime:
    s_n_vcd_parser__n_simulationTime: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulationTime;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_timeSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_timeSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_timeSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_timeSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_simulationTime;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_timeSpan:
    s_n_vcd_parser__n_span_start_timeSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_timeSpan;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = timeSpan;
      goto s_n_vcd_parser__n_simulationTime;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_inSimulation:
    s_n_vcd_parser__n_inSimulation: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_inSimulation;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob6, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_span_end_commandSpan_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_inSimulation;
        }
        case kMatchMismatch: {
          p++;
          goto s_n_vcd_parser__n_inSimulation;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_commandSpan_2:
    s_n_vcd_parser__n_span_start_commandSpan_2: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_commandSpan_2;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = commandSpan;
      goto s_n_vcd_parser__n_inSimulation;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_2:
    s_n_vcd_parser__n_simulation_2: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob5, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_vcd_parser__n_invoke_store_command_3;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_simulation_2;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_5:
    s_n_vcd_parser__n_simulation_5: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_5;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob8, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 9;
          goto s_n_vcd_parser__n_invoke_store_command_3;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_simulation_5;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_7:
    s_n_vcd_parser__n_simulation_7: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_7;
      }
      switch (*p) {
        case 'f': {
          p++;
          match = 10;
          goto s_n_vcd_parser__n_invoke_store_command_3;
        }
        default: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_6:
    s_n_vcd_parser__n_simulation_6: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_6;
      }
      switch (*p) {
        case 'f': {
          p++;
          goto s_n_vcd_parser__n_simulation_7;
        }
        case 'n': {
          p++;
          match = 11;
          goto s_n_vcd_parser__n_invoke_store_command_3;
        }
        default: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_8:
    s_n_vcd_parser__n_simulation_8: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_8;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob9, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_simulation_8;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_4:
    s_n_vcd_parser__n_simulation_4: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_4;
      }
      switch (*p) {
        case 'a': {
          p++;
          goto s_n_vcd_parser__n_simulation_5;
        }
        case 'o': {
          p++;
          goto s_n_vcd_parser__n_simulation_6;
        }
        case 'v': {
          p++;
          goto s_n_vcd_parser__n_simulation_8;
        }
        default: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_3:
    s_n_vcd_parser__n_simulation_3: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_3;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob7, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_simulation_4;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_simulation_3;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_9:
    s_n_vcd_parser__n_simulation_9: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_9;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob10, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_simulation_9;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation_1:
    s_n_vcd_parser__n_simulation_1: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulation_1;
      }
      switch (*p) {
        case 'c': {
          p++;
          goto s_n_vcd_parser__n_simulation_2;
        }
        case 'd': {
          p++;
          goto s_n_vcd_parser__n_simulation_3;
        }
        case 'e': {
          p++;
          goto s_n_vcd_parser__n_simulation_9;
        }
        default: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulationId:
    s_n_vcd_parser__n_simulationId: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulationId;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_idSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_idSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_idSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_idSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_simulationId;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_idSpan:
    s_n_vcd_parser__n_span_start_idSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_idSpan;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = idSpan;
      goto s_n_vcd_parser__n_simulationId;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulationVectorEnd:
    s_n_vcd_parser__n_simulationVectorEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulationVectorEnd;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_start_idSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_start_idSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_start_idSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_start_idSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_simulationVectorEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulationVector:
    s_n_vcd_parser__n_simulationVector: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulationVector;
      }
      switch (*p) {
        case '-': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case '0': {
          p++;
          match = 0;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case '1': {
          p++;
          match = 1;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'H': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'L': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'U': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'W': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'X': {
          p++;
          match = 2;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'Z': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'h': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'l': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'u': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'w': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'x': {
          p++;
          match = 2;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        case 'z': {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_onDigit;
        }
        default: {
          goto s_n_vcd_parser__n_simulationVectorEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_simulation:
    s_n_vcd_parser__n_simulation: {
      if (p == endp) {
        return s_n_vcd_parser__n_simulation;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_simulation;
        }
        case '#': {
          p++;
          match = 13;
          goto s_n_vcd_parser__n_invoke_store_command_2;
        }
        case '$': {
          p++;
          goto s_n_vcd_parser__n_simulation_1;
        }
        case '-': {
          p++;
          match = 28;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case '0': {
          p++;
          match = 14;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case '1': {
          p++;
          match = 15;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'B': {
          p++;
          match = 31;
          goto s_n_vcd_parser__n_invoke_store_command_5;
        }
        case 'H': {
          p++;
          match = 27;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'L': {
          p++;
          match = 25;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'R': {
          p++;
          match = 33;
          goto s_n_vcd_parser__n_invoke_store_command_5;
        }
        case 'U': {
          p++;
          match = 21;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'W': {
          p++;
          match = 23;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'X': {
          p++;
          match = 17;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'Z': {
          p++;
          match = 19;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'b': {
          p++;
          match = 30;
          goto s_n_vcd_parser__n_invoke_store_command_5;
        }
        case 'h': {
          p++;
          match = 26;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'l': {
          p++;
          match = 24;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'r': {
          p++;
          match = 32;
          goto s_n_vcd_parser__n_invoke_store_command_5;
        }
        case 'u': {
          p++;
          match = 20;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'w': {
          p++;
          match = 22;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'x': {
          p++;
          match = 16;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        case 'z': {
          p++;
          match = 18;
          goto s_n_vcd_parser__n_invoke_store_command_4;
        }
        default: {
          goto s_n_vcd_parser__n_error_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_inDeclarationEnd:
    s_n_vcd_parser__n_inDeclarationEnd: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_inDeclarationEnd;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob4, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_span_end_commandSpan_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_inDeclarationEnd;
        }
        case kMatchMismatch: {
          p++;
          goto s_n_vcd_parser__n_inDeclarationEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_commandSpan_1:
    s_n_vcd_parser__n_span_start_commandSpan_1: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_commandSpan_1;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = commandSpan;
      goto s_n_vcd_parser__n_inDeclarationEnd;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_4:
    s_n_vcd_parser__n_declaration_4: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_4;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob3, 13);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 8;
          goto s_n_vcd_parser__n_invoke_store_command_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_4;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeIdentifierEnd:
    s_n_vcd_parser__n_scopeIdentifierEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_scopeIdentifierEnd;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_scopeIdentifierSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_scopeIdentifierSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_scopeIdentifierSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_scopeIdentifierSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_scopeIdentifierEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_scopeIdentifierSpan:
    s_n_vcd_parser__n_span_start_scopeIdentifierSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_scopeIdentifierSpan;
      }
      state->_span_pos1 = (void*) p;
      state->_span_cb1 = scopeIdentifierSpan;
      goto s_n_vcd_parser__n_scopeIdentifierEnd;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeIdentifier:
    s_n_vcd_parser__n_scopeIdentifier: {
      if (p == endp) {
        return s_n_vcd_parser__n_scopeIdentifier;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_scopeIdentifier;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_scopeIdentifier;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_scopeIdentifier;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_scopeIdentifier;
        }
        default: {
          goto s_n_vcd_parser__n_span_start_scopeIdentifierSpan;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_1:
    s_n_vcd_parser__n_scopeTypeEnd_1: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_1;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob12, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_vcd_parser__n_invoke_store_type;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_scopeTypeEnd_1;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_3:
    s_n_vcd_parser__n_scopeTypeEnd_3: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_3;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob13, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_vcd_parser__n_invoke_store_type;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_scopeTypeEnd_3;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_4:
    s_n_vcd_parser__n_scopeTypeEnd_4: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_4;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob14, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_store_type;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_scopeTypeEnd_4;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_2:
    s_n_vcd_parser__n_scopeTypeEnd_2: {
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_2;
      }
      switch (*p) {
        case 'o': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_3;
        }
        case 'u': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_4;
        }
        default: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_5:
    s_n_vcd_parser__n_scopeTypeEnd_5: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_5;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob15, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 4;
          goto s_n_vcd_parser__n_invoke_store_type;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_scopeTypeEnd_5;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd_6:
    s_n_vcd_parser__n_scopeTypeEnd_6: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd_6;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob16, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 5;
          goto s_n_vcd_parser__n_invoke_store_type;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_scopeTypeEnd_6;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeTypeEnd:
    s_n_vcd_parser__n_scopeTypeEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_scopeTypeEnd;
      }
      switch (*p) {
        case 'b': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_1;
        }
        case 'f': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_2;
        }
        case 'm': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_5;
        }
        case 't': {
          p++;
          goto s_n_vcd_parser__n_scopeTypeEnd_6;
        }
        default: {
          goto s_n_vcd_parser__n_error_2;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_scopeType:
    s_n_vcd_parser__n_scopeType: {
      if (p == endp) {
        return s_n_vcd_parser__n_scopeType;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_scopeType;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_scopeType;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_scopeType;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_scopeType;
        }
        default: {
          goto s_n_vcd_parser__n_scopeTypeEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_commandSpan_3:
    s_n_vcd_parser__n_span_start_commandSpan_3: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_commandSpan_3;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = commandSpan;
      goto s_n_vcd_parser__n_scopeType;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_5:
    s_n_vcd_parser__n_declaration_5: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_5;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob11, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_store_command_6;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_5;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_6:
    s_n_vcd_parser__n_declaration_6: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_6;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob17, 8);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 4;
          goto s_n_vcd_parser__n_invoke_store_command;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_6;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_7:
    s_n_vcd_parser__n_declaration_7: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_7;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob18, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 5;
          goto s_n_vcd_parser__n_invoke_store_command;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_7;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varNameEnd:
    s_n_vcd_parser__n_varNameEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_varNameEnd;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_varNameSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_varNameSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_varNameSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_varNameSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_varNameEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_varNameSpan:
    s_n_vcd_parser__n_span_start_varNameSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_varNameSpan;
      }
      state->_span_pos1 = (void*) p;
      state->_span_cb1 = varNameSpan;
      goto s_n_vcd_parser__n_varNameEnd;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varName:
    s_n_vcd_parser__n_varName: {
      if (p == endp) {
        return s_n_vcd_parser__n_varName;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_varName;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_varName;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_varName;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_varName;
        }
        default: {
          goto s_n_vcd_parser__n_span_start_varNameSpan;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varIdEnd:
    s_n_vcd_parser__n_varIdEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_varIdEnd;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_varIdSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_varIdSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_varIdSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_varIdSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_varIdEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_varIdSpan:
    s_n_vcd_parser__n_span_start_varIdSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_varIdSpan;
      }
      state->_span_pos1 = (void*) p;
      state->_span_cb1 = varIdSpan;
      goto s_n_vcd_parser__n_varIdEnd;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varId:
    s_n_vcd_parser__n_varId: {
      if (p == endp) {
        return s_n_vcd_parser__n_varId;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_varId;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_varId;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_varId;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_varId;
        }
        default: {
          goto s_n_vcd_parser__n_span_start_varIdSpan;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varSizeEnd:
    s_n_vcd_parser__n_varSizeEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_varSizeEnd;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_span_end_varSizeSpan;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_span_end_varSizeSpan;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_span_end_varSizeSpan;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_span_end_varSizeSpan;
        }
        default: {
          p++;
          goto s_n_vcd_parser__n_varSizeEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_varSizeSpan:
    s_n_vcd_parser__n_span_start_varSizeSpan: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_varSizeSpan;
      }
      state->_span_pos1 = (void*) p;
      state->_span_cb1 = varSizeSpan;
      goto s_n_vcd_parser__n_varSizeEnd;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varSize:
    s_n_vcd_parser__n_varSize: {
      if (p == endp) {
        return s_n_vcd_parser__n_varSize;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_varSize;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_varSize;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_varSize;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_varSize;
        }
        default: {
          goto s_n_vcd_parser__n_span_start_varSizeSpan;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_1:
    s_n_vcd_parser__n_varTypeEnd_1: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_1;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob19, 4);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 1;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_1;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_2:
    s_n_vcd_parser__n_varTypeEnd_2: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_2;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob20, 6);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 2;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_2;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_3:
    s_n_vcd_parser__n_varTypeEnd_3: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_3;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob21, 8);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 3;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_3;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_8:
    s_n_vcd_parser__n_varTypeEnd_8: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_8;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob22, 3);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 5;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_8;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_7:
    s_n_vcd_parser__n_varTypeEnd_7: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_7;
      }
      switch (*p) {
        case 't': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_8;
        }
        default: {
          match = 4;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_6:
    s_n_vcd_parser__n_varTypeEnd_6: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_6;
      }
      switch (*p) {
        case 'l': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_7;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_5:
    s_n_vcd_parser__n_varTypeEnd_5: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_5;
      }
      switch (*p) {
        case 'a': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_6;
        }
        case 'g': {
          p++;
          match = 6;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_4:
    s_n_vcd_parser__n_varTypeEnd_4: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_4;
      }
      switch (*p) {
        case 'e': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_5;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_10:
    s_n_vcd_parser__n_varTypeEnd_10: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_10;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 7;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case '1': {
          p++;
          match = 8;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_9:
    s_n_vcd_parser__n_varTypeEnd_9: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_9;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob23, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_10;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_9;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_12:
    s_n_vcd_parser__n_varTypeEnd_12: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_12;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob24, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 9;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_12;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_15:
    s_n_vcd_parser__n_varTypeEnd_15: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_15;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob25, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 11;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_15;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_16:
    s_n_vcd_parser__n_varTypeEnd_16: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_16;
      }
      switch (*p) {
        case 'r': {
          p++;
          match = 12;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_17:
    s_n_vcd_parser__n_varTypeEnd_17: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_17;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob26, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 13;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_17;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_14:
    s_n_vcd_parser__n_varTypeEnd_14: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_14;
      }
      switch (*p) {
        case '0': {
          p++;
          match = 14;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case '1': {
          p++;
          match = 15;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case 'a': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_15;
        }
        case 'o': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_16;
        }
        case 'r': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_17;
        }
        default: {
          match = 10;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_13:
    s_n_vcd_parser__n_varTypeEnd_13: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_13;
      }
      switch (*p) {
        case 'i': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_14;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_11:
    s_n_vcd_parser__n_varTypeEnd_11: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_11;
      }
      switch (*p) {
        case 'i': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_12;
        }
        case 'r': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_13;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_19:
    s_n_vcd_parser__n_varTypeEnd_19: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_19;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob27, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 16;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_19;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_20:
    s_n_vcd_parser__n_varTypeEnd_20: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_20;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob28, 2);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 17;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_varTypeEnd_20;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_21:
    s_n_vcd_parser__n_varTypeEnd_21: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_21;
      }
      switch (*p) {
        case 'r': {
          p++;
          match = 18;
          goto s_n_vcd_parser__n_invoke_store_type_1;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd_18:
    s_n_vcd_parser__n_varTypeEnd_18: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd_18;
      }
      switch (*p) {
        case 'a': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_19;
        }
        case 'i': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_20;
        }
        case 'o': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_21;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varTypeEnd:
    s_n_vcd_parser__n_varTypeEnd: {
      if (p == endp) {
        return s_n_vcd_parser__n_varTypeEnd;
      }
      switch (*p) {
        case 'e': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_1;
        }
        case 'i': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_2;
        }
        case 'p': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_3;
        }
        case 'r': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_4;
        }
        case 's': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_9;
        }
        case 't': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_11;
        }
        case 'w': {
          p++;
          goto s_n_vcd_parser__n_varTypeEnd_18;
        }
        default: {
          goto s_n_vcd_parser__n_error_3;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_varType:
    s_n_vcd_parser__n_varType: {
      if (p == endp) {
        return s_n_vcd_parser__n_varType;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_varType;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_varType;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_varType;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_varType;
        }
        default: {
          goto s_n_vcd_parser__n_varTypeEnd;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_span_start_commandSpan_4:
    s_n_vcd_parser__n_span_start_commandSpan_4: {
      if (p == endp) {
        return s_n_vcd_parser__n_span_start_commandSpan_4;
      }
      state->_span_pos0 = (void*) p;
      state->_span_cb0 = commandSpan;
      goto s_n_vcd_parser__n_varType;
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_9:
    s_n_vcd_parser__n_declaration_9: {
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_9;
      }
      switch (*p) {
        case 'r': {
          p++;
          match = 6;
          goto s_n_vcd_parser__n_invoke_store_command_7;
        }
        default: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_10:
    s_n_vcd_parser__n_declaration_10: {
      llparse_match_t match_seq;
      
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_10;
      }
      match_seq = llparse__match_sequence_id(state, p, endp, llparse_blob29, 5);
      p = match_seq.current;
      switch (match_seq.status) {
        case kMatchComplete: {
          p++;
          match = 7;
          goto s_n_vcd_parser__n_invoke_store_command;
        }
        case kMatchPause: {
          return s_n_vcd_parser__n_declaration_10;
        }
        case kMatchMismatch: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_8:
    s_n_vcd_parser__n_declaration_8: {
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_8;
      }
      switch (*p) {
        case 'a': {
          p++;
          goto s_n_vcd_parser__n_declaration_9;
        }
        case 'e': {
          p++;
          goto s_n_vcd_parser__n_declaration_10;
        }
        default: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration_1:
    s_n_vcd_parser__n_declaration_1: {
      if (p == endp) {
        return s_n_vcd_parser__n_declaration_1;
      }
      switch (*p) {
        case 'c': {
          p++;
          goto s_n_vcd_parser__n_declaration_2;
        }
        case 'd': {
          p++;
          goto s_n_vcd_parser__n_declaration_3;
        }
        case 'e': {
          p++;
          goto s_n_vcd_parser__n_declaration_4;
        }
        case 's': {
          p++;
          goto s_n_vcd_parser__n_declaration_5;
        }
        case 't': {
          p++;
          goto s_n_vcd_parser__n_declaration_6;
        }
        case 'u': {
          p++;
          goto s_n_vcd_parser__n_declaration_7;
        }
        case 'v': {
          p++;
          goto s_n_vcd_parser__n_declaration_8;
        }
        default: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    case s_n_vcd_parser__n_declaration:
    s_n_vcd_parser__n_declaration: {
      if (p == endp) {
        return s_n_vcd_parser__n_declaration;
      }
      switch (*p) {
        case 9: {
          p++;
          goto s_n_vcd_parser__n_declaration;
        }
        case 10: {
          p++;
          goto s_n_vcd_parser__n_declaration;
        }
        case 13: {
          p++;
          goto s_n_vcd_parser__n_declaration;
        }
        case ' ': {
          p++;
          goto s_n_vcd_parser__n_declaration;
        }
        case '$': {
          p++;
          goto s_n_vcd_parser__n_declaration_1;
        }
        default: {
          goto s_n_vcd_parser__n_error_4;
        }
      }
      /* UNREACHABLE */;
      abort();
    }
    default:
      /* UNREACHABLE */
      abort();
  }
  s_n_vcd_parser__n_span_end_commandSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos0;
    state->_span_pos0 = NULL;
    err = commandSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_declaration;
      return s_error;
    }
    goto s_n_vcd_parser__n_declaration;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_commandSpan;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_error_4: {
    state->error = 0x1;
    state->reason = "Expected declaration command";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_timeSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos0;
    state->_span_pos0 = NULL;
    err = timeSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_simulation;
      return s_error;
    }
    goto s_n_vcd_parser__n_simulation;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_2: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_timeSpan;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_3: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_commandSpan_2;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_error_1: {
    state->error = 0x4;
    state->reason = "Expected simulation command";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_idSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos0;
    state->_span_pos0 = NULL;
    err = idSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_simulation;
      return s_error;
    }
    goto s_n_vcd_parser__n_simulation;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_4: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_idSpan;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_error: {
    state->error = 0x1;
    state->reason = "Content-Length overflow";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_onDigit: {
    switch (onDigit(state, p, endp, match)) {
      case 1:
        goto s_n_vcd_parser__n_error;
      default:
        goto s_n_vcd_parser__n_simulationVector;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_5: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_simulationVector;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_commandSpan_1: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos0;
    state->_span_pos0 = NULL;
    err = commandSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_simulation;
      return s_error;
    }
    goto s_n_vcd_parser__n_simulation;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_1: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_commandSpan_1;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_scopeIdentifierSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos1;
    state->_span_pos1 = NULL;
    err = scopeIdentifierSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_inDeclaration;
      return s_error;
    }
    goto s_n_vcd_parser__n_inDeclaration;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_type: {
    switch (vcd_parser__c_store_type(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_scopeIdentifier;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_error_2: {
    state->error = 0x2;
    state->reason = "Expected scope type";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_6: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_commandSpan_3;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_varNameSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos1;
    state->_span_pos1 = NULL;
    err = varNameSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_inDeclaration;
      return s_error;
    }
    goto s_n_vcd_parser__n_inDeclaration;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_varIdSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos1;
    state->_span_pos1 = NULL;
    err = varIdSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_varName;
      return s_error;
    }
    goto s_n_vcd_parser__n_varName;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_span_end_varSizeSpan: {
    const unsigned char* start;
    int err;
    
    start = state->_span_pos1;
    state->_span_pos1 = NULL;
    err = varSizeSpan(state, start, p);
    if (err != 0) {
      state->error = err;
      state->error_pos = (const char*) p;
      state->_current = (void*) (intptr_t) s_n_vcd_parser__n_varId;
      return s_error;
    }
    goto s_n_vcd_parser__n_varId;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_type_1: {
    switch (vcd_parser__c_store_type(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_varSize;
    }
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_error_3: {
    state->error = 0x3;
    state->reason = "Expected var type";
    state->error_pos = (const char*) p;
    state->_current = (void*) (intptr_t) s_error;
    return s_error;
    /* UNREACHABLE */;
    abort();
  }
  s_n_vcd_parser__n_invoke_store_command_7: {
    switch (vcd_parser__c_store_command(state, p, endp, match)) {
      default:
        goto s_n_vcd_parser__n_span_start_commandSpan_4;
    }
    /* UNREACHABLE */;
    abort();
  }
}

int vcd_parser_execute(vcd_parser_t* state, const char* p, const char* endp) {
  llparse_state_t next;

  /* check lingering errors */
  if (state->error != 0) {
    return state->error;
  }

  /* restart spans */
  if (state->_span_pos0 != NULL) {
    state->_span_pos0 = (void*) p;
  }
  if (state->_span_pos1 != NULL) {
    state->_span_pos1 = (void*) p;
  }
  
  next = vcd_parser__run(state, (const unsigned char*) p, (const unsigned char*) endp);
  if (next == s_error) {
    return state->error;
  }
  state->_current = (void*) (intptr_t) next;

  /* execute spans */
  if (state->_span_pos0 != NULL) {
    int error;
  
    error = ((vcd_parser__span_cb) state->_span_cb0)(state, state->_span_pos0, (const char*) endp);
    if (error != 0) {
      state->error = error;
      state->error_pos = endp;
      return error;
    }
  }
  if (state->_span_pos1 != NULL) {
    int error;
  
    error = ((vcd_parser__span_cb) state->_span_cb1)(state, state->_span_pos1, (const char*) endp);
    if (error != 0) {
      state->error = error;
      state->error_pos = endp;
      return error;
    }
  }
  
  return 0;
}