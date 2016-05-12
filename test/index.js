#!/usr/bin/env node

'use strict';

const EH = require('segfault-handler');
EH.registerHandler('crash.log');

try {
	const blake2 = require('bindings')('addon');

	// Blake2b
	const blake2b = blake2.createHash('blake2b');
	return;
	/*
		const blake2b_key = blake2.createHash('blake2b', {
			key: 'key',
			length: 64
		});
	//*/

	blake2b.update(new Buffer('00FF', 'hex'))

	console.log(blake2b.digest().toString('hex'));
} catch (err) {
	console.log("ERROR:", err);
}
