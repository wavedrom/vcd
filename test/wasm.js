'use strict';

const expect = require('chai').expect;
// const lib = require('../lib/index.js');

const dut = require('../lib/index.js');


describe('basic', () => {

  let wrapper;

  // return a promise from before and mocha
  // will wait for it
  before(() => {
    wrapper = dut.wrapper();
    return wrapper.start();
  });


  it('wasm basic', done => {

    console.log("test");

    const wires = {};
    const info = {stack: [wires], wires: wires};


    wrapper.init({},{},info);

    console.log(info);


    // wrapper.c.execute('hello world');

    wrapper.execute();

    expect(info.foo).to.equal(10);
    // console.log(wrapper.log());


    // expect(lib.parser).to.be.an('function');
    done();
  });


});

