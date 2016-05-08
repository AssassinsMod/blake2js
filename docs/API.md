# High-level API
## Using the module
```js
var blake2 = require('blake2');
```

## Creating a hash
```js
var hash = blake2.createHash([algorithm[, out_length]]);
```

The `algorithm` can either be:
- blake2b _(default)_
- blake2bp
- blake2s
- blake2sp

The `out_length` parameter is the final hash length and is an integer between 1 and:
- 64 for blake2b and blake2bp _(default)_
- 32 for blake2s and blake2sp


## Updating the hash
```js
hash.update([data[, encoding]]);
```

## Creating the digest
```js
hash.digest([encoding]);
```

## Pseudo-class
```js
class blake2 {
	constructor(algorithm = 'blake2b', out_length = 64) {
		this.algorithm = algorithm;
		<blake2::blake2X_init(&this._state, out_length)>;
	}

	update(data, encoding) {
		if (typeof data !== Buffer)
			data = new Buffer(data, encoding);

		<blake2::blakeX_update(&this._state, data)>
	}

	digest() {
		return <blake2::blake2X_final(&this._state)>
	}
}
```
