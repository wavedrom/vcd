'use strict';

const EventEmitter = require('events').EventEmitter;
const pkg = require('./package.json');
let lib = require('bindings')('vcd.node');

const version = pkg.version;

module.exports = () => {
  const lifee = new EventEmitter();
  const triee = new EventEmitter();

  const lifemit = lifee.emit.bind(lifee);
  const triemit = triee.emit.bind(triee);

  const info = {path: []};

  const cxt = lib.init(lifemit, triemit, info);

  const execute = chunk => lib.execute(cxt, lifemit, triemit, info, chunk);

  const on = (id, fn) => {
    lifee.on(id, fn);
  };

  const onTrigger = (id, fn) => {
    lib.setTrigger(cxt, id);
    triee.on(id, fn);
  };

  return {
    version,
    info,
    execute,
    on,
    onTrigger
  };
};
