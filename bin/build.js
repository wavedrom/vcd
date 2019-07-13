#!/usr/bin/env node
'use strict';

const fs = require('fs');
const llparse = require('llparse');
const llparseDot = require('llparse-dot');

const prj = 'vcd_parser';

const p = new llparse.LLParse(prj);

const start = p.node('start');
const stop = p.node('stop');

start
  .match('stop', stop)
  .otherwise(p.error(1, 'Expected start'));

stop
  .match('start', start)
  .otherwise(p.error(2, 'Expected start'));

// Build

const artifacts = p.build(start);

fs.writeFileSync(prj + '.h', artifacts.header);
// fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
fs.writeFileSync(prj + '.c', artifacts.c);

const dot = new llparseDot.Dot();

fs.writeFileSync(prj + '.dot', dot.build(start));

/* eslint camelcase: 0 */
