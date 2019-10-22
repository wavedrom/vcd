#!/usr/bin/env node
'use strict';

const fs = require('fs-extra');
const async = require('async');
const lib = require('../index.js');

const dir = './tmp/';

fs.readdir(dir).then(files => {
  // files.map(fileName => {
  async.eachSeries(files, (fileName, callback) => {
    let len = 0;
    let chunks = 0;
    let goodChunks = 0;
    const t0 = Date.now();
    const cxt = lib.init();
    lib.setTrigger(cxt, 'D1');
    const s = fs.createReadStream(dir + fileName);
    s.on('data', chunk => {
      if (lib.execute(cxt, chunk) === 0) {
        goodChunks++;
      }
      len += chunk.length;
      chunks++;
    });
    s.on('end', () => {
      console.log(
        fileName,
        chunks,
        len,
        goodChunks,
        lib.getStop(cxt) - lib.getStart(cxt),
        ((Date.now() - t0) / 1000 + 's')
      );
      callback();
    });
  });
});
