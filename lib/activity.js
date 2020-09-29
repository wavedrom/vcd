'use strict';

module.exports = skip => {
  let start = 0;
  let stop = 0;
  let up = 0;
  let total = 0;
  return {
    on: (time, cmd) => {
      if (time > skip) {
        if (start == 0) {
          start = time;
        } else {
          stop = time;
        }
      }
      if (cmd === 15) {
        up = time;
      } else
      if (cmd === 14) {
        total += (time - up); up = time;
      }
    },
    time: () => stop - start,
    uptime: () => total
  };
};
