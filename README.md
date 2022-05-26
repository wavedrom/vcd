[![Linux NAPI](https://github.com/wavedrom/vcd/actions/workflows/linux-napi.yml/badge.svg)](https://github.com/wavedrom/vcd/actions/workflows/linux-napi.yml)[![Linux WASM](https://github.com/wavedrom/vcd/actions/workflows/linux-wasm.yml/badge.svg)](https://github.com/wavedrom/vcd/actions/workflows/linux-wasm.yml)[![MacOS NAPI](https://github.com/wavedrom/vcd/actions/workflows/macos-napi.yml/badge.svg)](https://github.com/wavedrom/vcd/actions/workflows/macos-napi.yml)[![MacOS WASM](https://github.com/wavedrom/vcd/actions/workflows/macos-wasm.yml/badge.svg)](https://github.com/wavedrom/vcd/actions/workflows/macos-wasm.yml)[![Windows WASM](https://github.com/wavedrom/vcd/actions/workflows/windows-wasm.yml/badge.svg)](https://github.com/wavedrom/vcd/actions/workflows/windows-wasm.yml)
[![NPM version](https://img.shields.io/npm/v/vcd-stream.svg)](https://www.npmjs.org/package/vcd-stream)

Value Change Dump ([VCD](https://en.wikipedia.org/wiki/Value_change_dump)) parser using [llparse](https://github.com/nodejs/llparse)

## Usage

Install

```
npm i vcd-stream
make wasm
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

### Build / Test Napi version

```
npm i
npm run mocha_napi
```

### Build / Test Wasm version

```
make
npm run mocha_wasm
```

### Test all

```
npm test
```

## License

MIT [LICENSE](LICENSE)
