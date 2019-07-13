'use strict';

const pkg = require('./package.json');

let lib = require('bindings')('vpreproc.node');
lib.version = pkg.version;

module.exports = lib;
