'use strict';

function _waitForStart(mod) {
  return new Promise((resolve, reject)=>{
    mod.addOnPostRun(resolve);
  });
}

module.exports = () => {
  // let state = 0; // idle
  // let total = 0;
  // let start = 0;

  const wasm = require('../out/vcd.js');


  let start = async() => {
    await _waitForStart(wasm);
  }

  return {
    start,
    log: () => {
      console.log(wasm);
    },
    onB: (time, cmd) => {
      
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
