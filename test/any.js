'use strict';

const expect = require('chai').expect;
const lib = require('../lib/index.js');

describe('any', () => {

  it('simple', done => {
    const inst = lib.parser();
    const dump = [];
    inst.change.any((id, time, cmd, value, mask) => {
      dump.push({
        id, time, cmd, value, mask
      });
    });

    inst.on('finish', () => {
      expect(dump).to.deep.eq([
        { id: 'ABC', time: 100, cmd: 14, value: 0n, mask: 0n },
        { id: '123', time: 200, cmd: 15, value: 1n, mask: 0n },
        { id: 'XyZ', time: 300, cmd: 14, value: 0n, mask: 0n },
        { id: 'foo', time: 400, cmd: 15, value: 1n, mask: 0n },
      ])
      // console.log(dump);
      done();
    });

    inst.write(`\
$enddefinitions $end
#100
0ABC
#200
1123
#300
0XyZ
#400
1foo
`);
    inst.end();
  });
});

/* eslint-env mocha */
