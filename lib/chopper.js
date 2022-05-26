'use strict';

function xoshiro128ss(a, b, c, d) {
  return function() {
    var t = b << 9, r = a * 5; r = (r << 7 | r >>> 25) * 9;
    c ^= a; d ^= b;
    b ^= c; a ^= d; c ^= t;
    d = d << 11 | d >>> 21;
    return (r >>> 0) / 4294967296;
  };
}


function * chopper (src, step) {

  const chunker = len => {
    const chunk = src.slice(0, len);
    src = src.slice(chunk.length);
    return chunk;
  };

  const random = xoshiro128ss(1134534345, 2145245245624, 313442566456, 4245642456456);
  // const random = Math.random;
  // const random = () => 0;
  // yield(chunker(395));
  // yield(chunker(145));
  while(true) {
    if (src.length === 0) {
      return;
    }
    yield(chunker(random() * step + step));
  }
}

module.exports = chopper;
