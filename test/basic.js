'use strict';

const expect = require('chai').expect;
const lib = require('../lib/index.js');

describe('basic', () => {

  it('typeof vcd', done => {
    expect(lib.parser).to.be.an('function');
    done();
  });

  it('typeof vcd instance', done => {
    expect(lib.parser()).to.be.an('object');
    done();
  });

  it('fail: foo bar', done => {
    const inst = lib.parser();
    expect(inst.write(Buffer.from(' foo bar ???'))).to.eq(true);
    expect(inst.info).to.deep.eq({
      stack: [{}],
      status: 'declaration',
      wires: {}
    });
    done();
  });

  it('$comment', done => {
    const inst = lib.parser();
    expect(inst.write(Buffer.from(
      ' \n $comment some text $end $comment more text $end ???'
    ))).to.eq(true);
    expect(inst.info).to.deep.eq({
      stack: [{}],
      status: 'declaration',
      wires: {}
    });
    done();
  });

  it('$version', done => {
    const inst = lib.parser();
    expect(inst.write(`
$version Generated by VerilatedVcd $end
$date Wed Sep 18 22:59:07 2019
 $end
$timescale   1ns $end

  $scope   module   top    $end
    $var wire  1 "}G clock $end
    $scope module leaf $end
      $var wire 64 {u counter [63:0] $end
    $upscope $end
    $scope module fruit $end
      $var wire 4 u) point [3:0] $end
    $upscope $end
  $upscope $end

  $enddefinitions $end
`
)).to.eq(true);

    expect(inst.write(`

#1
0"}G
#2
1"}G
#300
0"}G
b1111000000000000 {u
#301
b0000111100000000 {u
#302
b0000000011110000 {u
#303
b0000000000001111 {u
`
)).to.eq(true);

    expect(inst.info).to.deep.eq({
      status: 'simulation',
      varId: 'u)',
      wires: {
        top: {
          clock: '"}G',
          fruit: {
            point: 'u)'
          },
          leaf: {
            counter: '{u'
          }
        }
      },
      stack: [{
        top: {
          clock: '"}G',
          fruit: {
            point: 'u)'
          },
          leaf: {
            counter: '{u'
          }
        }
      },
      {
        clock: '"}G',
        fruit: {
          point: 'u)'
        },
        leaf: {
          counter: '{u'
        }
      },
      {
        point: 'u)'
      }]
    });
    done();
  });
});

/* eslint-env mocha */
