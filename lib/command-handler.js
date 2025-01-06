'use strict';

const handleScope = (info, str) => {
  const [type, name] = str.split(/\s+/);
  const ero = {kind: 'scope', type, name, body: []};
  const current = info.stack[info.stack.length - 1];
  current.body.push(ero);
  info.stack.push(ero);
  // console.log(ero);
};

const handleUpScope = (info /* , str */) => {
  info.stack.pop();
  // console.log(['upscope', str]);
};

const updateIdTable = ($, link) => {
  if ($.ido[link] !== undefined) { // old Id
    return;
  }

  // polynomial rolling hash function
  let hash = 0;
  let poly = 1;
  for (let i = 0; i < link.length; i++) {
    const c = link.charCodeAt(i) - 33 + 1; // ! .. ~ (94 digits)
    hash = (hash + poly * c) & 0xfff;
    poly = (poly * 97) & 0xfff; // 89, 97
  }

  $.ido[link] = [$.nextId, hash];

  // add entry to the Hash Table object
  if ($.hashTable[hash] === undefined) {
    $.hashTable[hash] = {};
  }

  $.hashTable[hash][link] = $.nextId;
  $.nextId += 1;
};

const handleVar = (info, str) => {
  // reg 3 ( r_reg [2:0]
  // 0   1 2 3+
  const eroj = str.split(/\s+/);
  const link = eroj[2];
  updateIdTable(info, link);
  const ero = {
    kind: 'var',
    type: eroj[0],
    size: parseInt(eroj[1]),
    link,
    name: eroj.slice(3).join('')
  };
  {
    const m = ero.name.match('^(?<name>\\w+)\\[' + (ero.size - 1) + ':0]$');
    if (m) {
      ero.name = m.groups.name;
    }
  }
  const current = info.stack[info.stack.length - 1];
  current.body.push(ero);
  // console.log(ero);
};

const commandHandler = (info, cmd, str) => {
  str = str.trim();
  switch(cmd) {
  case 1:
    info.comment = str;
    // console.log(['comment', str]);
    break;
  case 2:
    info.date = str;
    // console.log(['date', str]);
    break;
  case 3:
    handleScope(info, str);
    break;
  case 4:
    info.timescale = str;
    // console.log(['timescale', str]);
    break;
  case 5:
    handleUpScope(info, str);
    break;
  case 6:
    handleVar(info, str);
    break;
  case 7:
    info.version = str;
    // console.log(['version', str]);
    break;
  default:
    console.log([cmd, str]);
  }
};

module.exports = commandHandler;
