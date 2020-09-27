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
    c.execute = w('execute', 'void',   ['number', 'number', 'number', 'number', 'number', 'string']);
    c.init    = w('init',    'number', ['number', 'number', 'number', 'number']);
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

  let boundInfo;

  let boundSet;
  let boundGet;

  let context = -1;

  // wasm.addFunction can't be called until after
  // start finishes
  bindCallback = () => {
    boundSet = wasm.addFunction(function(name, len, type, v0, v1) {

      let prop = getString(name, len);

      switch(type) {
        case 0:
          boundInfo[prop] = v0;
          break;
        case 1:
          boundInfo[prop] = getString(v0, v1);
          break;
        default: throw new Error();
      }

      console.log(`setting ${prop} to ${boundInfo[prop]}`);

      // viiiii means returns void, accepts int int int int int
    }, 'viiiii');

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
    init: (cb0, cb1, info) => {
      boundInfo = info;
      context = c.init(0,0,boundSet,boundGet);
    },
    execute: () => {
      c.execute(context,0,0,boundSet,boundGet,"hi");
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
