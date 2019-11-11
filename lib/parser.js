'use strict';

const stream = require('stream');
const EventEmitter = require('events').EventEmitter;
let lib = require('bindings')('vcd.node');

module.exports = () => {

  const triggers = [];

  const info = {path: []};

  const s = new stream.Writable();

  // const lifee = new EventEmitter();
  const lifemit = s.emit.bind(s);

  const triee = new EventEmitter();
  const triemit = triee.emit.bind(triee);

  const cxt = lib.init(lifemit, triemit, info);

  s._write = function (chunk, encoding, callback) {
    lib.execute(cxt, lifemit, triemit, info, chunk);
    callback();
  };

  s.change = {
    on: (id, fn) => {
      triggers.push(id);
      const triggerString = triggers.join(' ') + '  ';
      lib.setTrigger(cxt, triggerString);
      triee.on(id, fn);
    }
  };

  s.info = info;

  return s;
};
