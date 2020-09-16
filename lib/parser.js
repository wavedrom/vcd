'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;
let lib = require('bindings')('vcd.node');

module.exports = () => {

  const wires = {};
  const info = {stack: [wires], wires: wires};

  const s = new stream.Writable();

  // const lifee = new EventEmitter();
  const lifemit = s.emit.bind(s);

  const triee = new EventEmitter();
  const triemit = triee.emit.bind(triee);
  let triemit2 = triemit;

  const cxt = lib.init(lifemit, triemit, info);

  s._write = function (chunk, encoding, callback) {
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

  return s;
};
