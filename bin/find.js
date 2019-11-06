#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const async = require('async');

const vcd = require('../index.js');

const dir = './tmp/';

fs.readdir(dir).then(files => {
  const tt0 = Date.now();
  async.eachLimit(files, 1, (fileName, callback) => {

    let start = 0;
    let stop = 0;
    const t0 = Date.now();

    let inst = vcd();

    // inst.on('$enddefinitions', () => {
      // console.log(res);
      // console.log(inst.info);
    // });

    inst.onTrigger('D1', time => {
      if (time > 10) {
        if (start == 0) {
          start = time;
        } else {
          stop = time;
        }
      }
    });

    const s = fs.createReadStream(dir + fileName);

    s.on('data', inst.execute);

    s.on('end', () => {
      console.log(fileName, (stop - start), ((Date.now() - t0) / 1000 + 's'));
      callback();
    });
  }, () => {
    console.log('Total time: ' + (Date.now() - tt0) / 1000 + 's');
  });
});
