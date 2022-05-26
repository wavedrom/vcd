'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;
let lib = require('bindings')('vcd.node');

module.exports = () => {

  const wires = {};
  const info = {stack: [wires], wires: wires};

  const s = new stream.Writable();

  // const lifee = new EventEmitter();

  // gets called by c with 1 argument, a number
  const lifemit = s.emit.bind(s);

  const triee = new EventEmitter();

  // gets called by c with 5 arguments
  // string        eventName
  // bigint        state->time
  // int           command
  // bigint        state->value
  // bigint        state->mask

  const triemit = triee.emit.bind(triee);
  let triemit2 = triemit;

  const cxt = lib.init(lifemit, triemit, info);

  s._write = function (chunk, encoding, callback) {
    const err = lib.execute(cxt, lifemit, triemit2, info, chunk);
    if (err) {
      // console.log(info);
      console.log(err);
      // throw new Error(err);
    }
    callback();
  };

  s.change = {
    on: (id, fn) => {
      // console.log(id);
      triemit2 = triemit;
      triee.on(id, fn);
      const triggerString = triee.eventNames().join('\0') + '\0\0';
      lib.setTrigger(cxt, triggerString);
    },
    any: fn => {
      triemit2 = fn;
      lib.setTrigger(cxt, '\0');
    }
  };

  s.info = info;

  s.getTime = () => lib.getTime(cxt);

  s.kill = () => {
    lib.done(cxt, lifemit, triemit2, info);
    s.end();
  };

  return s;
};
