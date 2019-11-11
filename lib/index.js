'use strict';

const pkg = require('../package.json');
const parser = require('./parser.js');
const and = require('./and.js');
const activity = require('./activity.js');

module.exports = {
  version: pkg.version,
  and: and,
  activity: activity,
  parser: parser
};
