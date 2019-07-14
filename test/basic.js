'use strict';

const expect = require('chai').expect;
const lib = require('../index.js');

describe('basic', () => {
  it('type', done => {
    expect(lib).to.be.an('object');
    done();
  });
  it('type', done => {
    const cxt = lib.init();
    expect(cxt).to.be.an('object');
    console.log(cxt);
    done();
  });
});

/* eslint-env mocha */
