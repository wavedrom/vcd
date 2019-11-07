#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const async = require('async');

const vcd = require('../index.js');

const dir = './tmp/';

fs.readdir(dir).then(files => {
  const tt0 = Date.now();
  async.eachLimit(files, 2, (fileName, callback) => {

    let start = 0;
    let stop = 0;
    const t0 = Date.now();

    let inst = vcd();

    inst.on('$enddefinitions', () => {
      // console.log(res);
      // console.log(inst.info);
      inst.onTrigger('D1', time => {
        if (time > 10) {
          if (start == 0) {
            start = time;
          } else {
            stop = time;
          }
        }
      });
    });

    inst.on('finish', () => {
      console.log(fileName, (stop - start), ((Date.now() - t0) / 1000 + 's'));
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
