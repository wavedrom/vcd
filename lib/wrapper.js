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
    c.execute    = w('execute',    'number', ['number', 'number', 'number', 'number', 'number', 'string']);
    c.init       = w('init',       'number', ['number', 'number', 'number', 'number']);
    c.getTime    = w('getTime',    'number', ['number']);
    c.setTrigger = w('setTrigger', 'number', ['number', 'string']);
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

  let ee = [];

  let boundEE0;
  let boundEE1;

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

    boundEE0 = wasm.addFunction(function(val) {
      ee[0].emit(val);
    }, 'vi');

    boundEE1 = wasm.addFunction(function(eventName, l0, time, command, value, mask) {
      ee[1].emit(getString(eventName, l0), time, command, value, mask);
    }, 'viiiiii');

  };

  return {
    start,
    c,
    log: () => {
      console.log(wasm);
    },
    init: (cb0, cb1, info) => {
      boundInfo = info;
      ee[0] = cb0;
      ee[1] = cb1;
      context = c.init(boundEE0,boundEE1,boundSet,boundGet);
    },
    execute: (ctx, lifemit, triemit, info, chunk) => {
      c.execute(ctx,0,0,boundSet,boundGet,chunk.toString());
    },
    setTrigger: (ctx, triggerString) => {
      return c.setTrigger(ctx, triggerString);
    },
    getTime: (ctx) => {
      return c.getTime(ctx);
    },
    time: () => total + start
  };
};
