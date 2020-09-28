'use strict';

const dotProp = require('dot-prop');

function _waitForStart(mod) {
  return new Promise((resolve, reject)=>{
    mod.addOnPostRun(resolve);
  });
}

function u8ToBn(u8) {
  var hex = [];
  // let u8 = Uint8Array.from(buf);

  u8.forEach(function (i) {
    var h = i.toString(16);
    if (h.length % 2) { h = '0' + h; }
    hex.push(h);
  });

  hex.reverse();

  return BigInt('0x' + hex.join(''));
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
        // set number
        case 0:
          boundInfo[prop] = v0;
          // console.log(`setting ${prop} to ${boundInfo[prop]}`);
          break;
        // set string
        case 1:
          boundInfo[prop] = getString(v0, v1);
          // console.log(`setting ${prop} to ${boundInfo[prop]}`);
          break;
        // set string to path
        case 2:
          dotProp.set(boundInfo, prop, getString(v0, v1));
          // console.log(`setting ${prop} to ${getString(v0, v1)}`);
          break;
        // path to path (any type)
        case 3:
          let tmp = dotProp.get(boundInfo, getString(v0, v1));
          // console.log(`for ${getString(v0, v1)} got ${tmp}, set to ${prop}`);
          dotProp.set(boundInfo, prop, tmp);
          break;
        // create empty object at path
        case 4:
          // console.log(`${prop} is new {}`);
          dotProp.set(boundInfo, prop, {});
          break;

        default: throw new Error();
      }


      // viiiii means returns void, accepts int int int int int
    }, 'viiiii');

    boundGet = wasm.addFunction(function(name, len) {
      let prop = getString(name, len);
      return 42;
    }, 'iii');


    boundEE0 = wasm.addFunction(function(name, len) {
      ee[0](getString(name, len));
    }, 'vii');

    // const char* name, const size_t len, const uint64_t time, const uint8_t command, const int valueWords, const uint64_t* aValue, const uint64_t* aMask);
    // boundEE1 = wasm.addFunction(function(eventName, l0, time, command, valueWords, value, mask) {
    boundEE1 = wasm.addFunction(function(eventName, l0, time, command, valueWords, value, mask) {
      const name = getString(eventName, l0);
      // console.log(`event name`);
      // console.log(`event ${name} time ${time} cmd ${command} wrds ${valueWords}`);


      const view0 = wasm.HEAPU8.subarray(value, value+(valueWords*4));
      const view1 = wasm.HEAPU8.subarray(mask,  mask+(valueWords*4));

      let big_value = u8ToBn(view0);
      let big_mask = u8ToBn(view1);

      // console.log(big_value.toString(16));

      ee[1](name, time, command, big_value, big_mask);
    }, 'viiiiiii');

  };

  return {
    start,
    c,
    init: (cb0, cb1, info) => {
      boundInfo = info;
      ee[0] = cb0;
      ee[1] = cb1;
      context = c.init(boundEE0,boundEE1,boundSet,boundGet);
    },
    execute: (ctx, lifemit, triemit, info, chunk) => {
      c.execute(ctx,boundEE0,boundEE1,boundSet,boundGet,chunk.toString());
    },
    setTrigger: (ctx, triggerString) => {
      return c.setTrigger(ctx, triggerString);
    },
    getTime: (ctx) => {
      return BigInt(c.getTime(ctx));
    },
    time: () => total + start
  };
};
