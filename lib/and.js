'use strict';

module.exports = () => {
  let state = 0; // idle
  let total = 0;
  let start = 0;

  return {
    onA: (time, cmd) => {
      switch(state) {
      case 0: if (cmd === 15) { state = 1; } break;
      case 1: if (cmd === 14) { state = 0; } break;
      case 2: if (cmd === 15) { state = 3; start = time; } break;
      case 3: if (cmd === 14) { state = 2; total += (time - start); start = 0; } break;
      default: throw new Error();
      }
    },
    onNotA: (time, cmd) => {
      switch(state) {
      case 0: if (cmd === 14) { state = 1; } break;
      case 1: if (cmd === 15) { state = 0; } break;
      case 2: if (cmd === 14) { state = 3; start = time; } break;
      case 3: if (cmd === 15) { state = 2; total += (time - start); start = 0; } break;
      default: throw new Error();
      }
    },
    onB: (time, cmd) => {
      switch(state) {
      case 0: if (cmd === 15) { state = 2; } break;
      case 1: if (cmd === 15) { state = 3; start = time; } break;
      case 2: if (cmd === 14) { state = 0; } break;
      case 3: if (cmd === 14) { state = 1; total += (time - start); start = 0; } break;
      default: throw new Error();
      }
    },
    onNotB: (time, cmd) => {
      switch(state) {
      case 0: if (cmd === 14) { state = 2; } break;
      case 1: if (cmd === 14) { state = 3; start = time; } break;
      case 2: if (cmd === 15) { state = 0; } break;
      case 3: if (cmd === 15) { state = 1; total += (time - start); start = 0; } break;
      default: throw new Error();
      }
    },
    time: () => total + start
  };
};
