#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const cp = require('child_process');
const llparse = require('llparse');

const gyp = cb => {
  console.log('build');
  const proc = cp.spawn('node-gyp', ['configure', 'build']);
  proc.stderr.on('data', data => {
    console.error(data.toString());
  });
  proc.on('close', (cb || (() => {
    console.log('done');
  })));
};

const objection = lut => arg => arg.split(/\s+/).reduce((res, key) => {
  if (lut[key] === undefined) {
    throw new Error(key);
  }
  res[key] = lut[key];
  return res;
}, {});

const properties = {
  command:      'i8',
  type:         'i8',
  size:         'i32',
  time:         'i64', // current simulation time
  trigger:      'ptr',
  triee:        'ptr', // trigger event emitter
  lifee:        'ptr', // life cycle event emmiter
  info:         'ptr',
  value:        'ptr', // value of the signal on change event
  mask:         'ptr', // mask (x, z) of the signal on change event
  digitCount:   'i32',
  tmpStr:       'ptr',
  tmpStr2:      'ptr',
  stackPointer: 'i32',
  id:           'ptr',
  napi_env:     'ptr'
};

const generate = cb => {
  // const llparseDot = require('llparse-dot');

  const prj = 'vcd_parser';
  const p = new llparse.LLParse(prj);

  Object.keys(properties).map(key => p.property(properties[key], key));

  const {
    scopeIdentifierSpan,
    varSizeSpan, varIdSpan, varNameSpan,
    idSpan,
    commandSpan,
    timeSpan
  } = `
    scopeIdentifierSpan
    varSizeSpan varIdSpan varNameSpan
    idSpan
    commandSpan
    timeSpan
  `
    .trim().split(/\s+/)
    .reduce((res, n) => Object.assign(res, {[n]: p.span(p.code.span(n))}), {});

  const {
    declaration,
    scopeType, scopeTypeEnd,
    scopeIdentifier, scopeIdentifierEnd,
    varType, varTypeEnd,
    varSize, varSizeEnd,
    varId, varIdEnd,
    varName, varNameEnd,
    inDeclaration,
    simulation,
    inSimulation,
    simulationTime,
    simulationVector, simulationVectorEnd,
    simulationId
  } = `
    declaration
    scopeType scopeTypeEnd
    scopeIdentifier scopeIdentifierEnd
    varType varTypeEnd
    varSize varSizeEnd
    varId varIdEnd
    varName varNameEnd
    inDeclaration
    simulation
    inSimulation
    simulationTime
    simulationVector simulationVectorEnd
    simulationId
  `
    .trim().split(/\s+/)
    .reduce((res, n) => Object.assign(res, {[n]: p.node(n)}), {});

  const enddefinitions = p.node('inDeclarationEnd');

  const spaces = [' ', '\n', '\r', '\t'];

  const cmd = objection({
    $comment: 1,
    $date: 2,
    $scope: 3,
    $timescale: 4,
    $upscope: 5,
    $var: 6,
    $version: 7,
    $enddefinitions: 8,
    $dumpall: 9,
    $dumpoff: 10,
    $dumpon: 11,
    $dumpvars: 12,
    '#': 13,
    '0': 14, '1': 15, x: 16, X: 17, Z: 18,
    b: 19, B: 20, r: 21, R: 22
  });

  declaration
    .match(spaces, declaration)
    .select(cmd('$scope'),
      p.invoke(p.code.store('command'), commandSpan.start(scopeType)))
    .select(cmd('$var'),
      p.invoke(p.code.store('command'), commandSpan.start(varType)))
    .select(cmd('$comment $date $timescale $upscope $version'),
      p.invoke(p.code.store('command'), commandSpan.start(inDeclaration)))
    .select(cmd('$enddefinitions'),
      p.invoke(p.code.store('command'), commandSpan.start(enddefinitions)))
    .otherwise(p.error(1, 'Expected declaration command'));

  // $scope

  scopeType
    .match(spaces, scopeType)
    .otherwise(scopeTypeEnd);

  scopeTypeEnd
    .select({begin: 1, fork: 2, function: 3, module: 4, task: 5},
      p.invoke(p.code.store('type'), scopeIdentifier))
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
      real: 4,
      realtime: 5,
      reg: 6,
      supply0: 7,
      supply1: 8,
      time: 9,
      tri: 10,
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
    .match(spaces, varIdSpan.end(varName))
    .skipTo(varIdEnd);

  varName
    .match(spaces, varName)
    .otherwise(varNameSpan.start(varNameEnd));

  varNameEnd
    .match(spaces, varNameSpan.end(inDeclaration))
    .skipTo(varNameEnd);

  // $end

  inDeclaration
    .match('$end', commandSpan.end(declaration))
    .skipTo(inDeclaration);

  enddefinitions
    .match('$end', commandSpan.end(simulation))
    .skipTo(enddefinitions);

  simulation
    .match([' ', '\n', '\t'], simulation)
    .select(cmd('$dumpall $dumpoff $dumpon $dumpvars $comment'),
      p.invoke(p.code.store('command'), commandSpan.start(inSimulation)))
    .select(cmd('#'),
      p.invoke(p.code.store('command'), timeSpan.start(simulationTime)))
    .select(cmd('0 1 x X Z'),
      p.invoke(p.code.store('command'), idSpan.start(simulationId)))
    .select(cmd('b B r R'),
      p.invoke(p.code.store('command'), simulationVector))
    .otherwise(p.error(4, 'Expected simulation command'));

  inSimulation
    .match('$end', commandSpan.end(simulation))
    .skipTo(inSimulation);

  simulationTime
    .match(spaces, timeSpan.end(simulation))
    .skipTo(simulationTime);

  simulationVector
    .select(
      {0: 0, 1: 1, x: 2, z: 3, X: 2, Z: 3},
      p.invoke(
        // p.code.mulAdd('value', {base: 2, signed: false}),
        p.code.value('onDigit'),
        {1: p.error(1, 'Content-Length overflow')},
        simulationVector
      )
    )
    .otherwise(simulationVectorEnd);

  simulationVectorEnd
    .match(spaces, idSpan.start(simulationId))
    .skipTo(simulationVectorEnd);

  simulationId
    .match(spaces, idSpan.end(simulation))
    .skipTo(simulationId);

  const artifacts = p.build(declaration);

  fs.writeFileSync(prj + '.h', artifacts.header);
  // fs.writeFileSync('verilog_preprocessor.bc', artifacts.bitcode);
  fs.writeFileSync(prj + '.c', artifacts.c);

  // const dot = new llparseDot.Dot();
  // fs.writeFileSync(prj + '.dot', dot.build(declaration));

  cb();
};

generate(gyp);

/* eslint camelcase: 0 */
