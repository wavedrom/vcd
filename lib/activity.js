'use strict';

module.exports = skip => {
  let start = 0;
  let stop = 0;
  return {
    on: time => {
      if (time > skip) {
        if (start == 0) {
          start = time;
        } else {
          stop = time;
        }
      }
    },
    time: () => stop - start
  };
};
