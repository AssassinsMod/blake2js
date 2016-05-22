#!/usr/bin/env node

'use strict';

const addon = require('bindings')('addon');

//*

// Blake2s
console.log('Blake2s  ', addon
	.createHash('blake2s')
	.update(new Buffer('blake2s'))
	.digest()
	.toString('hex')
);

// Blake2sp
console.log('Blake2sp ', addon
	.createHash('blake2sp')
	.update(new Buffer('blake2sp'))
	.digest()
	.toString('hex')
);

// Blake2b
console.log('Blake2b  ', addon
	.createHash('blake2b')
	.update(new Buffer('blake2b'))
	.digest()
	.toString('hex')
);

// Blake2bp
console.log('Blake2bp ', addon
	.createHash('blake2bp')
	.update(new Buffer('blake2bp'))
	.digest()
	.toString('hex')
);

//*/
