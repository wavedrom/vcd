'use strict';

const pkg = require('../package.json');
const parser = require('./parser.js');
const wasmparser = require('./wasmparser.js');
const and = require('./and.js');
const activity = require('./activity.js');
const wrapper = require('./wrapper.js');

module.exports = {
  version: pkg.version,
  and: and,
  activity: activity,
  parser: parser,
  wasmparser: wasmparser,
  wrapper: wrapper
};
