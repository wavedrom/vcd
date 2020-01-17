[![NPM version](https://img.shields.io/npm/v/vcd-stream.svg)](https://www.npmjs.org/package/vcd-stream)
[![Actions Status](https://github.com/wavedrom/vcd/workflows/Node/badge.svg)](https://github.com/wavedrom/vcd/actions)

Value Change Dump ([VCD](https://en.wikipedia.org/wiki/Value_change_dump)) parser using [llparse](https://github.com/nodejs/llparse)

## Usage

Install

```
npm i vcd-stream
```

Require

```js
let vcd = require('vcd-stream');
```

Create parser writable stream instance

```js
let inst = vcd.parser();
```

General event emitter

```js
inst.on(<eventName>, () => {});
```

Events:
* `$enddefinitions` - when all modules/wires are defined
* `finish` - end of stream
* `error` - error during parsing process

Change event emitter

```js
inst.change.on(<wireName>, (time, cmd) => {});
```

* `time` -- change time
* `cmd` -- change type

Info object

```js
let info = inst.info;
```

* `info.status` - (`'declaration'`|`'simulation'`)
* `info.wires` - hierarchy object of modules and wires

Pipe data into the instance

```js
myStream.pipe(inst);
```

## Test

```
npm i
npm test
```

## License

MIT [LICENSE](LICENSE)
