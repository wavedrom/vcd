#!/usr/bin/env node
'use strict';

const EventEmitter = require('events').EventEmitter;

const fs = require('fs-extra');
const async = require('async');

const lib = require('../index.js');

const dir = './tmp/';

fs.readdir(dir).then(files => {
  const tt0 = Date.now();
  async.eachLimit(files, 1, (fileName, callback) => {
    let len = 0;
    let chunks = 0;
    let goodChunks = 0;
    let start = 0;
    let stop = 0;
    const t0 = Date.now();

    const ee = new EventEmitter();

    const cxt = lib.init();
    lib.setTrigger(cxt, 'D1');
    ee.on('trigger', time => {
      if (time < 10) {
        return 0;
      }
      if (start == 0) {
        start = time;
      } else {
        stop = time;
      }
    });

    const s = fs.createReadStream(dir + fileName);
    s.on('data', chunk => {
      if (lib.execute(cxt, ee.emit.bind(ee), chunk) === 0) {
        goodChunks++;
      }
      len += chunk.length;
      chunks++;
    });
    s.on('end', () => {
      // const info = lib.getInfo(cxt);
      // console.log(info);
      console.log(fileName, chunks, len, goodChunks, (stop - start),
        ((Date.now() - t0) / 1000 + 's')
      );
      callback();
    });
  }, () => {
    console.log('Total time: ' + (Date.now() - tt0) / 1000 + 's');
  });
});
