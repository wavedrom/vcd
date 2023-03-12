#!/usr/bin/env node
'use strict';

const fs = require('fs');
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
  maskCount:    'i32',
  tmpStr:       'ptr',
  timeStampStr: 'ptr',
  idStr:        'ptr',
  tmpStr2:      'ptr',
  stackPointer: 'i32',
  id:           'ptr',
  napi_env:     'ptr'
};

const spaces = [' ', '\n', '\r', '\t'];
const lineSpaces = [' ', '\t'];

const generate = (cb) => {
  // const llparseDot = require('llparse-dot');

  const prj = 'vcd_parser';
  const p = new llparse.LLParse(prj);

  Object.keys(properties).map(key => p.property(properties[key], key));

  const {
    // scopeIdentifierSpan,
    varSizeSpan, varIdSpan, varNameSpan,
    idSpan,
    commandSpan,
    timeSpan
  } = `
    varSizeSpan varIdSpan varNameSpan
    idSpan
    commandSpan
    timeSpan
  `
    .trim().split(/\s+/)
    .reduce((res, n) => Object.assign(res, {[n]: p.span(p.code.span(n))}), {});

  // scopeIdentifierSpan

  const {
    declaration,
    // scopeType, scopeTypeEnd,
    // scopeIdentifier, scopeIdentifierEnd,
    varType, varTypeEnd,
    varSize, varSizeEnd,
    varId, varIdEnd,
    varName, varNameEnd,
    inDeclaration,
    simulation,
    inSimulation,
    simulationTime,
    simulationVector, simulationVectorEnd, simulationVectorRecovery,
    simulationId
  } = `
    declaration
    varType varTypeEnd
    varSize varSizeEnd
    varId varIdEnd
    varName varNameEnd
    inDeclaration
    simulation
    inSimulation
    simulationTime
    simulationVector simulationVectorEnd simulationVectorRecovery
    simulationId
  `
    .trim().split(/\s+/)
    .reduce((res, n) => Object.assign(res, {[n]: p.node(n)}), {});

  // scopeType scopeTypeEnd
  // scopeIdentifier scopeIdentifierEnd

  const enddefinitions = p.node('inDeclarationEnd');

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
    '0': 14, '1': 15,
    x: 16, X: 17,
    z: 18, Z: 19,
    u: 20, U: 21, // VHDL states
    w: 22, W: 23,
    l: 24, L: 25,
    h: 26, H: 27,
    '-': 28,
    b: 30, B: 31, r: 32, R: 33
  });

  declaration
    .match(spaces, declaration)
    // .select(cmd('$scope'),
    //   p.invoke(p.code.store('command'), commandSpan.start(scopeType)))
    // .select(cmd('$var'),
    //   p.invoke(p.code.store('command'), commandSpan.start(varType)))
    .select(cmd('$scope $var $upscope $comment $date $timescale $version'),
      p.invoke(p.code.store('command'), commandSpan.start(inDeclaration)))
    .select(cmd('$enddefinitions'),
      p.invoke(p.code.store('command'), commandSpan.start(enddefinitions)))
    .otherwise(p.error(1, 'Expected declaration command'));

  // $scope module clkdiv2n_tb $end
  //        ^^^^^^

  // scopeType.match(spaces, scopeType).otherwise(scopeTypeEnd);
  // scopeTypeEnd
  //   .select(
  //     {
  //       module: 0,
  //       task: 1,
  //       function: 2,
  //       begin: 3,
  //       fork: 4,
  //       // extra scopes from Verilator
  //       generate: 5,
  //       struct: 6,
  //       union: 7,
  //       class: 8,
  //       interface: 9,
  //       package: 10,
  //       program: 11
  //     },
  //     p.invoke(p.code.store('type'), scopeIdentifier))
  //   .otherwise(p.error(2, 'Expected scope type'));

  // $scope module clkdiv2n_tb $end
  //               ^^^^^^^^^^^

  // scopeIdentifier.match(spaces, scopeIdentifier).otherwise(scopeIdentifierSpan.start(scopeIdentifierEnd));
  // scopeIdentifierEnd.match(spaces, scopeIdentifierSpan.end(inDeclaration)).skipTo(scopeIdentifierEnd);

  // $var reg 3 ( r_reg [2:0] $end
  //      ^^^

  varType.match(spaces, varType).otherwise(varTypeEnd);
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

  // $var reg 3 ( r_reg [2:0] $end
  //          ^

  varSize.match(spaces, varSize).otherwise(varSizeSpan.start(varSizeEnd));
  varSizeEnd.match(spaces, varSizeSpan.end(varId)).skipTo(varSizeEnd);

  // $var reg 3 ( r_reg [2:0] $end
  //            ^

  varId.match(spaces, varId).otherwise(varIdSpan.start(varIdEnd));
  varIdEnd.match(spaces, varIdSpan.end(varName)).skipTo(varIdEnd);

  // $var reg 3 ( r_reg [2:0] $end
  //              ^^^^^

  varName.match(spaces, varName).otherwise(varNameSpan.start(varNameEnd));
  varNameEnd.match('$end', commandSpan.end(varNameSpan.end(declaration))).skipTo(varNameEnd);

  // $end

  inDeclaration
    .match('$end', commandSpan.end(declaration))
    .skipTo(inDeclaration);

  enddefinitions
    .match('$end', commandSpan.end(simulation))
    .skipTo(enddefinitions);

  simulation
    .match([' ', '\r', '\n', '\t', '$dumpvars', '$end'], simulation)
    .select(cmd('$dumpall $dumpoff $dumpon $comment'),
      p.invoke(p.code.store('command'), commandSpan.start(inSimulation)))
    .select(cmd('#'),
      p.invoke(p.code.store('command'), timeSpan.start(simulationTime)))
    .select(cmd('0 1 x X z Z u U w W l L h H -'),
      p.invoke(p.code.store('command'), idSpan.start(simulationId)))
    .select(cmd('b B r R'),
      p.invoke(p.code.store('command'), simulationVector))
    .otherwise(p.error(4, 'Expected simulation command'));

  inSimulation
    .match('$end', commandSpan.end(simulation))
    .skipTo(inSimulation);

  simulationTime
    .match(spaces, timeSpan.end(p.invoke(p.code.span('onTime'), simulation)))
    .skipTo(simulationTime);

  simulationVector
    .select(
      {
        0: 0,
        1: 1,
        x: 2, X: 2,
        z: 3, Z: 3,
        u: 3, U: 3, // VHDL states
        w: 3, W: 3,
        l: 3, L: 3,
        h: 3, H: 3,
        '-': 3
      },
      p.invoke(
        // p.code.mulAdd('value', {base: 2, signed: false}),
        p.code.value('onDigit'),
        {1: p.error(5, 'Content-Length overflow')},
        simulationVector
      )
    )
    .otherwise(simulationVectorEnd);

  simulationVectorEnd
    .match(lineSpaces, idSpan.start(simulationId))
    .skipTo(simulationVectorRecovery);

  simulationVectorRecovery
    .select(
      {
        '\n': 1, '\r': 1
      },
      p.invoke(
        p.code.value('onRecover'),
        {1: p.error(6, 'recover')},
        simulation
      )
    )
    .skipTo(simulationVectorRecovery);

  simulationId
    .match(spaces, idSpan.end(p.invoke(p.code.span('onId'), simulation)))
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
