#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const async = require('async');

const lib = require('../lib/index.js');

const dir = './tmp/';

fs.readdir(dir).then(files => {
  const tt0 = Date.now();
  async.eachLimit(files, 2, (fileName, callback) => {
    if (!fileName.match(/.vcd$/)) {
      callback();
      return;
    }

    const t0 = Date.now();

    let inst = lib.parser();

    const loads = lib.and();
    const stores = lib.and();
    const duration = lib.activity(10);

    inst.on('$enddefinitions', () => {
      // console.log(res);
      // console.log(inst.info);
      inst.change.on('D7', (time, cmd) => { // mem_i_load
        loads.onA(time, cmd);
        stores.onNotA(time, cmd);
      });
      inst.change.on('D1', (time, cmd) => { // mem_i_valid
        loads.onB(time, cmd);
        stores.onB(time, cmd);
        duration.on(time);
      });
    });

    inst.on('finish', () => {
      console.log(fileName, duration.time(), loads.time(), stores.time(), ((Date.now() - t0) / 1000 + 's'));
      callback();
    });

    inst.on('error', err => {
      console.log(err);
    });

    fs.createReadStream(dir + fileName).pipe(inst);

  }, () => {
    console.log('Total time: ' + (Date.now() - tt0) / 1000 + 's');
  });
});
