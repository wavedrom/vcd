'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;

const dotProp = require('dot-prop');

function _waitForStart(mod) {
  return new Promise((resolve)=>{
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

let startCalled = 0;

const getWrapper = wasm => {

  const c = {};

  let bindCallback;

  const bindCWrap = () => {
    const w = wasm.cwrap;
    c.execute    = w('execute',    'number', ['number', 'number', 'number', 'number', 'number', 'string']);
    c.init       = w('init',       'number', ['number', 'number', 'number', 'number']);
    c.getTime    = w('getTime',    'number', ['number']);
    c.setTrigger = w('setTrigger', 'number', ['number', 'string']);
  };

  const start = async() => {
    // if( !startCalled ) {
    //   await _waitForStart(wasm);
    //   startCalled++;
    // }
    // console.log('s1');
    bindCWrap();
    // console.log('s2');
    bindCallback();
    // console.log('s3');
  };

  // gets a string from a c heap pointer and length
  const getString = (name, len) => {
    const view = wasm.HEAPU8.subarray(name, name+len);

    let string = '';
    for (let i = 0; i < len; i++) {
      string += String.fromCharCode(view[i]);
    }
    return string;
  };

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
      let tmp;

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
        tmp = dotProp.get(boundInfo, getString(v0, v1));
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
      return prop;
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


      const view0 = wasm.HEAPU8.subarray(value, value+(valueWords*8));
      const view1 = wasm.HEAPU8.subarray(mask,  mask+(valueWords*8));

      let bigValue = u8ToBn(view0);
      let bigMask = u8ToBn(view1);

      // console.log(bigValue.toString(16));

      ee[1](name, time, command, bigValue, bigMask);
    }, 'viiiiiii');

  };

  return {
    start,
    c,
    init: (cb0, cb1, info) => {
      boundInfo = info;
      ee[0] = cb0;
      ee[1] = cb1;
      context = c.init(boundEE0, boundEE1, boundSet, boundGet);
      return context;
    },
    execute: (ctx, cb0, cb1, info, chunk) => {
      boundInfo = info;
      ee[0] = cb0;
      ee[1] = cb1;
      c.execute(ctx, boundEE0, boundEE1, boundSet, boundGet, chunk.toString());
    },
    setTrigger: (ctx, triggerString) => {
      return c.setTrigger(ctx, triggerString);
    },
    getTime: (ctx) => {
      return BigInt(c.getTime(ctx));
    }
  };

};

module.exports = async wasm => {
  const lib = getWrapper(wasm);
  // console.log('getWrapper', lib);
  await lib.start();
  // console.log('vcd wasm srarted');

  const wires = {};
  const info = {stack: [wires], wires: wires};

  const s = new stream.Writable();

  // gets called by c with 1 argument, a string
  const lifemit = s.emit.bind(s);

  const triee = new EventEmitter();

  // gets called by c with 5 arguments
  // string        eventName
  // number        state->time
  // int           command
  // int           state->value
  // int           state->mask

  const triemit = triee.emit.bind(triee);
  let triemit2 = triemit;

  const cxt = lib.init(lifemit, triemit, info);

  s._write = function (chunk, encoding, callback) {
    // console.log('about to write', chunk);
    lib.execute(cxt, lifemit, triemit2, info, chunk);
    // console.log(util.inspect(info, {showHidden: true, depth : null, colorize: true}));
    // console.log(info.stack[0].top);
    // console.log(info.stack[1]);
    // console.log(info.stack[0].top == info.stack[1]);
    callback();
  };

  s.change = {
    on: (id, fn) => {
      triemit2 = triemit;
      // console.log(id, fn);
      triee.on(id, fn);
      const triggerString = triee.eventNames().join(' ') + '  ';
      lib.setTrigger(cxt, triggerString);
    },
    any: fn => {
      triemit2 = fn;
      lib.setTrigger(cxt, '\0');
    }
  };

  s.info = info;

  s.getTime = () => lib.getTime(cxt);

  s.start = lib.start;

  return s;
};

/* global BigInt */
