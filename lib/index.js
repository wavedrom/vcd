'use strict';

const pkg = require('../package.json');
const parser = require('./parser.js');
const wasmparser = require('./wasmparser.js');
const and = require('./and.js');
const activity = require('./activity.js');
const wrapper = require('./wrapper.js');
const webVcdParser = require('./web-vcd-parser.js');

module.exports = {
  version: pkg.version,
  and,
  activity,
  parser,
  wasmparser,
  wrapper,
  webVcdParser
};
