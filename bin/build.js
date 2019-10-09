#!/usr/bin/env node
'use strict';

const fs = require('fs');
const llparse = require('llparse');
const llparseDot = require('llparse-dot');

const prj = 'vcd_parser';

const p = new llparse.LLParse(prj);

const declaration = p.node('declaration');
const inDeclaration = p.node('inDeclaration');
const enddefinitions = p.node('inDeclarationEnd');
const simulation = p.node('simulation');
const inSimulation = p.node('inSimulation');

// Add custom uint8_t property to the state
p.property('i8', 'declaration');
p.property('i8', 'simulation');

// Store method inside a custom property
const onDeclaration = p.invoke(p.code.store('declaration'), inDeclaration);
const onSimulation = p.invoke(p.code.store('simulation'), inSimulation);

declaration
  .select({
    '$comment': 0,
    '$date': 1,
    '$scope': 2,
    '$timescale': 3,
    '$upscope': 4,
    '$var': 5,
    '$version': 6
  }, onDeclaration)
  .match('$enddefinitions', enddefinitions)
  .otherwise(p.error(1, 'Expected declaration'));

inDeclaration
  .match('$end', declaration)
  .otherwise(p.error(2, 'Expected end of declaration'));

enddefinitions
  .match('$end', simulation)
  .otherwise(p.error(3, 'Expected end of all declaration'));

simulation
  .select({
    '$dumpall': 0,
    '$dumpoff': 1,
    '$dumpon': 2,
    '$dumpvars': 3,
    '$comment': 4
  }, onSimulation)
  .otherwise(p.error(4, 'Expected simulation command'));

inSimulation
  .match('$end', simulation)
  .otherwise(p.error(5, 'Expected end simulation command'));

// Build

const artifacts = p.build(declaration);

fs.writeFileSync(prj + '.h', artifacts.header);
// fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
fs.writeFileSync(prj + '.c', artifacts.c);

const dot = new llparseDot.Dot();

fs.writeFileSync(prj + '.dot', dot.build(declaration));

/* eslint camelcase: 0 */
