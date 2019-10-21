#!/usr/bin/env node
'use strict';

const fs = require('fs');
const llparse = require('llparse');
const llparseDot = require('llparse-dot');

const prj = 'vcd_parser';
const p = new llparse.LLParse(prj);

// Add custom uint8_t property to the state
p.property('i8', 'command');
p.property('i8', 'type');
p.property('i32', 'size');

const declaration = p.node('declaration');

const scopeType = p.node('scopeType');
const scopeTypeEnd = p.node('scopeTypeEnd');

const scopeIdentifier = p.node('scopeIdentifier');
const scopeIdentifierSpan = p.span(p.code.span('scopeIdentifierSpan'));
const scopeIdentifierEnd = p.node('scopeIdentifierEnd');

const varType = p.node('varType');
const varTypeEnd = p.node('varTypeEnd');

const varSize = p.node('varSize');
const varSizeSpan = p.span(p.code.span('varSizeSpan'));
const varSizeEnd = p.node('varSizeEnd');

const varId = p.node('varId');
const varIdSpan = p.span(p.code.span('varIdSpan'));
const varIdEnd = p.node('varIdEnd');

const commandSpan = p.span(p.code.span('commandSpan'));
const inDeclaration = p.node('inDeclaration');
const enddefinitions = p.node('inDeclarationEnd');
const simulation = p.node('simulation');
const inSimulation = p.node('inSimulation');
const simulationTime = p.node('simulationTime');

const spaces = [' ', '\n', '\r', '\t'];

declaration
  .match(spaces, declaration)
  .select({$scope: 3}, p.invoke(p.code.store('command'), commandSpan.start(scopeType)))
  .select({$var: 6},   p.invoke(p.code.store('command'), commandSpan.start(varType)))
  .select({
    $comment: 1, $date: 2, $timescale: 4, $upscope: 5, $version: 7
  }, p.invoke(p.code.store('command'), commandSpan.start(inDeclaration)))
  .select({
    $enddefinitions: 8
  }, p.invoke(p.code.store('command'), commandSpan.start(enddefinitions)))
  .otherwise(p.error(1, 'Expected declaration command'));

// $scope

scopeType
  .match(spaces, scopeType)
  .otherwise(scopeTypeEnd);

scopeTypeEnd
  .select({
    begin: 1, fork: 2, function: 3, module: 4, task: 5
  }, p.invoke(p.code.store('type'), scopeIdentifier))
  .otherwise(p.error(2, 'Expected scope type'));

scopeIdentifier
  .match(spaces, scopeIdentifier)
  .otherwise(scopeIdentifierSpan.start(scopeIdentifierEnd));

scopeIdentifierEnd
  .match(spaces, scopeIdentifierSpan.end(inDeclaration))
  .skipTo(scopeIdentifierEnd);

// $var

varType
  .match(spaces, varType)
  .otherwise(varTypeEnd);

varTypeEnd
  .select({
    event: 1,
    integer: 2,
    parameter: 3,
    // real: 4,
    realtime: 5,
    reg: 6,
    supply0: 7,
    supply1: 8,
    time: 9,
    // tri: 10,
    triand: 11,
    trior: 12,
    trireg: 13,
    tri0: 14,
    tri1: 15,
    wand: 16,
    wire: 17,
    wor: 18
  }, p.invoke(p.code.store('type'), varSize))
  .otherwise(p.error(3, 'Expected var type'));

varSize
  .match(spaces, varSize)
  .otherwise(varSizeSpan.start(varSizeEnd));

varSizeEnd
  .match(spaces, varSizeSpan.end(varId))
  .skipTo(varSizeEnd);

varId
  .match(spaces, varId)
  .otherwise(varIdSpan.start(varIdEnd));

varIdEnd
  .match(spaces, varIdSpan.end(inDeclaration))
  .skipTo(varIdEnd);

// $end

inDeclaration
  .match('$end', commandSpan.end(declaration))
  .skipTo(inDeclaration);

enddefinitions
  .match('$end', commandSpan.end(simulation))
  .skipTo(enddefinitions);

simulation
  .match([' ', '\n', '\t'], simulation)
  .select({
    $dumpall: 9, $dumpoff: 10, $dumpon: 11, $dumpvars: 12, $comment: 1
  }, p.invoke(p.code.store('command'), commandSpan.start(inSimulation)))
  .select({'#': 13}, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .select({
    '0': 14, '1': 15, x: 16, X: 17, Z: 18
  }, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .select({}, p.invoke(p.code.store('command'), commandSpan.start(simulationTime)))
  .otherwise(p.error(4, 'Expected simulation command'));

inSimulation
  .match('$end', commandSpan.end(simulation))
  .skipTo(inSimulation);

simulationTime
  .match(spaces, commandSpan.end(simulation))
  .skipTo(simulationTime);


// Build

const artifacts = p.build(declaration);

fs.writeFileSync(prj + '.h', artifacts.header);
// fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
fs.writeFileSync(prj + '.c', artifacts.c);

const dot = new llparseDot.Dot();

fs.writeFileSync(prj + '.dot', dot.build(declaration));

/* eslint camelcase: 0 */
