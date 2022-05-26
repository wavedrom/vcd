'use strict';

const fs = require('fs');
const path = require('path');

const expect = require('chai').expect;
const createVCD = require('../out/vcd.js');
const webVcdParser = require('../lib/web-vcd-parser.js');
const chopper = require('../lib/chopper.js');

const expectaitions = [
  { id: '"}G', time: 100n, cmd: 14, value: 0n, mask: 0n },
  { id: '"}G', time: 200n, cmd: 15, value: 1n, mask: 0n },
  { id: '{u', time: 200n, cmd: 30, value: 0xf0f0f0f0f0f0f0f0n, mask: 0xff00ff00ff00ff00n },
  { id: '"}G', time: 300n, cmd: 14, value: 0n, mask: 0n },
  { id: '{u', time: 300n, cmd: 30, value: 0xf000000000000000n, mask: 0n },
  { id: 'u)', time: 300n, cmd: 30, value: 0n, mask: 0n },
  { id: '{u', time: 301n, cmd: 30, value: 0x0f00000000000000n, mask: 0n },
  { id: 'u)', time: 301n, cmd: 30, value: 1n, mask: 0n },
  { id: '{u', time: 302n, cmd: 30, value: 0x00f0000000000000n, mask: 0n },
  { id: 'u)', time: 302n, cmd: 30, value: 2n, mask: 0n },
  { id: '{u', time: 303n, cmd: 30, value: 0x000f000000000000n, mask: 0n },
  { id: 'u)', time: 303n, cmd: 30, value: 3n, mask: 0n },
  { id: '{u', time: 304n, cmd: 30, value: 0x0000f00000000000n, mask: 0n },
  { id: 'u)', time: 304n, cmd: 30, value: 4n, mask: 0n },
  { id: '{u', time: 305n, cmd: 30, value: 0x00000f0000000000n, mask: 0n },
  { id: 'u)', time: 305n, cmd: 30, value: 5n, mask: 0n },
  { id: '{u', time: 306n, cmd: 30, value: 0x000000f000000000n, mask: 0n },
  { id: 'u)', time: 306n, cmd: 30, value: 6n, mask: 0n },
  { id: '{u', time: 307n, cmd: 30, value: 0x0000000f00000000n, mask: 0n },
  { id: 'u)', time: 307n, cmd: 30, value: 7n, mask: 0n },
  { id: '{u', time: 308n, cmd: 31, value: 0x00000000f0000000n, mask: 0n },
  { id: 'u)', time: 308n, cmd: 30, value: 8n, mask: 0n },
  { id: '{u', time: 309n, cmd: 30, value: 0x000000000f000000n, mask: 0n },
  { id: 'u)', time: 309n, cmd: 30, value: 9n, mask: 0n },
  { id: '{u', time: 310n, cmd: 30, value: 0x0000000000f00000n, mask: 0n },
  { id: 'u)', time: 310n, cmd: 30, value: 10n, mask: 0n },
  { id: '{u', time: 311n, cmd: 30, value: 0x00000000000f0000n, mask: 0n },
  { id: 'u)', time: 311n, cmd: 30, value: 11n, mask: 0n },
  { id: '{u', time: 312n, cmd: 30, value: 0x000000000000f000n, mask: 0n },
  { id: 'u)', time: 312n, cmd: 30, value: 12n, mask: 0n },
  { id: '{u', time: 313n, cmd: 30, value: 0x0000000000000f00n, mask: 0n },
  { id: 'u)', time: 313n, cmd: 30, value: 13n, mask: 0n },
  { id: '{u', time: 314n, cmd: 30, value: 0x00000000000000f0n, mask: 0n },
  { id: 'u)', time: 314n, cmd: 30, value: 14n, mask: 0n },
  { id: '{u', time: 315n, cmd: 30, value: 0x000000000000000fn, mask: 0n },
  { id: 'u)', time: 315n, cmd: 30, value: 15n, mask: 0n },
  { id: '"}G', time: 316n, cmd: 15, value: 1n, mask: 0n }
];

describe('wasm dump', () => {
  it('simple wasm', done => {
    fs.readFile(path.join(__dirname, 'dump.vcd'), function (err, src) {
      if (err) {
        throw new Error(err);
      }
      createVCD().then((mod) => {
        webVcdParser(mod).then((inst) => {
          const dump = [];
          ['"}G', '{u', 'u)'] // array of all signal ids
            .map(id =>
              inst.change.on(id, (time, cmd, value, mask) => {
                const row = {
                  id,
                  time,
                  cmd,
                  value,
                  mask
                };
                dump.push(row);
                // console.log(row);
              })
            );

          inst.on('finish', () => {
            expect(inst.getTime()).to.eq(316n);
            // console.log(dump);
            // expect(dump.length).to.eq(expectaitions.length);
            expect(dump).to.deep.eq(expectaitions);
            done();
          });
          const step = (Math.random() * 500) |0;
          for (const chunk of chopper(src, step)) {
            // console.log('\n\u001b[31m[\u001b[0m' + chunk.toString() + '\u001b[31m](\u001b[0m\n' + chunk.length + '\u001b[31m)\u001b[0m\n');
            inst.write(chunk);
          }
          inst.end();
          // console.log(step);
        });
      });
    });
  });
});

/* eslint-env mocha */
