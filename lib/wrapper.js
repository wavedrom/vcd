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

  const c = {};

  const wasm = require('../out/vcd.js');

  let bindCallback;

  const bindCWrap = () => {
    const w = wasm.cwrap;
    c.execute = w('execute', 'void', ['number', 'number', 'number', 'number', 'number', 'string']);
  };

  const start = async() => {
    await _waitForStart(wasm);
    bindCWrap();
    bindCallback();
  }

  // gets a string from a c heap pointer and length
  const getString = (name,len) => {
     const view = wasm.HEAPU8.subarray(name, name+len);

     let string = '';
     for (let i = 0; i < len; i++) {
        string += String.fromCharCode(view[i]);
     }
     return string;
  }

  let boundSet;
  let boundGet;

  // wasm.addFunction can't be called until after
  // start finishes
  bindCallback = () => {
    boundSet = wasm.addFunction(function(name, len, value) {

      let prop = getString(name, len);

      console.log(`setting ${prop} to ${value}`);

      // viii means returns void, accepts int int int
    }, 'viii');

    boundGet = wasm.addFunction(function(name, len) {
      let prop = getString(name, len);
      return 42;
   }, 'iii');
  };

  return {
    start,
    c,
    log: () => {
      console.log(wasm);
    },
    execute: () => {
      c.execute(0,0,0,boundSet,boundGet,"hi");
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
