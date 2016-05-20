# Blake2js
> BLAKE2 is a cryptographic hash function faster than MD5, SHA-1, SHA-2, and SHA-3, yet is at least as secure as the latest standard SHA-3. — [blake2.net][1]

**Blake2js** is a *native Nodejs module* wrapped around [**BLAKE2's official C implementation**][2]. It's aim is to provide a simple yet complete javascript API in an object oriented flavour (similarly to the official [crypto api][3]).

[1]: https://blake2.net/
[2]: https://github.com/BLAKE2/BLAKE2
[3]: https://nodejs.org/api/crypto.html

## How to use
First you have to install locally the module:
```bash
$ npm install [--save|--save-dev] blake2js
```

Then inside your script you can require it as follows:
```javascript
var b2 = require('blake2js');
```

### Module API
The module itself exposes only two methods: `createHash` and `createHmac`. Both of them return an **object** whose api is described in the [Instance API section][4].

#### createHash & createHmac
```javascript
createHash(algorithm[, length])
createHmac(algorithm, key[, length])
```

The **algorithm** must be a *valid algorithm string*. Valid algorithms are: `blake2s`, `blake2sp`, `blake2b`, `blake2bp`.

The **key** can either be a buffer or an utf-8 string must be smaller or equal to the maximum key length of the choosen algorithm (32 for `blake2s` and 64 for `blake2b`).

The **length** must be an *integer* smaller or equal to the maximum output length of the choosen algorithm (32 for `blake2s` and 64 for `blake2b`).
