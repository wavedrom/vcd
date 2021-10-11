'use strict';

const expect = require('chai').expect;
const createVCD = require('../out/vcd.js');
const webVcdParser = require('../lib/web-vcd-parser.js');

describe('wasm events', () => {

  it('$enddefinitions', async function () {
    const mod = await createVCD();
    const inst = await webVcdParser(mod);

    inst.on('$enddefinitions', () => {
      expect(inst.info).to.deep.eq({
        status: 'simulation',
        timescale: '   1ns ',
        date: ' Wed Sep 18 22:59:07 2019\n ',
        version: ' Generated by VerilatedVcd ',
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
    });
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
  });


});

/* eslint-env mocha */
