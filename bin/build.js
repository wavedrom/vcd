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
const simulationTime = p.node('simulationTime');

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
    '$dumpall': 8, '$dumpoff': 9, '$dumpon': 10, '$dumpvars': 11,
    '$comment': 1
  }, p.invoke(p.code.store('command'), commandSpan.start(inSimulation)))
  .select({'#': 12}, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .select({'0': 13}, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .select({'1': 14}, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .otherwise(p.error(2, 'Expected simulation command'));

inSimulation
  .match('$end', commandSpan.end(simulation))
  .skipTo(inSimulation);

simulationTime
  .match([' ', '\n', '\r', '\t'], commandSpan.end(simulation))
  .skipTo(simulationTime);


// Build

const artifacts = p.build(declaration);

fs.writeFileSync(prj + '.h', artifacts.header);
// fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
fs.writeFileSync(prj + '.c', artifacts.c);

const dot = new llparseDot.Dot();

fs.writeFileSync(prj + '.dot', dot.build(declaration));

/* eslint camelcase: 0 */
