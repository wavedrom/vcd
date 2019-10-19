#!/usr/bin/env node
'use strict';

const fs = require('fs');
const llparse = require('llparse');
const llparseDot = require('llparse-dot');

const prj = 'vcd_parser';
const p = new llparse.LLParse(prj);

// Add custom uint8_t property to the state
p.property('i8', 'command');

const declaration = p.node('declaration');
const commandSpan = p.span(p.code.span('commandSpan'));
const inDeclaration = p.node('inDeclaration');
const enddefinitions = p.node('inDeclarationEnd');
const simulation = p.node('simulation');
const inSimulation = p.node('inSimulation');

declaration
  .match([' ', '\n', '\t'], declaration)
  .select({
    '$comment': 1, '$date': 2, '$scope': 3, '$timescale': 4,
    '$upscope': 5, '$var': 6, '$version': 7
  }, p.invoke(p.code.store('command'), commandSpan.start(inDeclaration)))
  .select({
    '$enddefinitions': 100
  }, p.invoke(p.code.store('command'), commandSpan.start(enddefinitions)))
  .otherwise(p.error(1, 'Expected declaration command'));

inDeclaration
  .match('$end', commandSpan.end(declaration))
  .skipTo(inDeclaration);

enddefinitions
  .match('$end', commandSpan.end(simulation))
  .skipTo(enddefinitions);

simulation
  .match([' ', '\n', '\t'], simulation)
  .select({
    '$dumpall': 101, '$dumpoff': 102, '$dumpon': 103, '$dumpvars': 104,
    '$comment': 1
  }, p.invoke(p.code.store('command'), commandSpan.start(inSimulation)))
  .otherwise(p.error(2, 'Expected simulation command'));

inSimulation
  .match('$end', commandSpan.end(simulation))
  .skipTo(inSimulation);

// Build

const artifacts = p.build(declaration);

fs.writeFileSync(prj + '.h', artifacts.header);
// fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
fs.writeFileSync(prj + '.c', artifacts.c);

const dot = new llparseDot.Dot();

fs.writeFileSync(prj + '.dot', dot.build(declaration));

/* eslint camelcase: 0 */
