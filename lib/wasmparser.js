'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;
// let lib = require('bindings')('vcd.node');


module.exports = async () => {

  const lib = require('../lib/wrapper.js')();
  await lib.start();

  const wires = {};
  const info = {stack: [wires], wires: wires};

  const s = new stream.Writable();

  
  // gets called by c with 1 argument, a number
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
    console.log('about to write', chunk);
    lib.execute(cxt, lifemit, triemit2, info, chunk);
    callback();
  };

  s.change = {
    on: (id, fn) => {
      triemit2 = triemit;
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
