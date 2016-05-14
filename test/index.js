#!/usr/bin/env node

'use strict';

const addon = require('bindings')('addon');

/*
const EH = require('segfault-handler');
EH.registerHandler('crash.log');
//*/

const blake2s = addon.createHash('blake2s');
blake2s.update(new Buffer('blake2s', 'utf-8'));
console.log('0x' + blake2s.digest().toString('hex'));

const blake2sp = addon.createHash('blake2sp');
blake2sp.update(new Buffer('blake2sp', 'utf-8'));
console.log('0x' + blake2sp.digest().toString('hex'));

const blake2b = addon.createHash('blake2b');
blake2b.update(new Buffer('blake2b', 'utf-8'));
console.log('0x' + blake2b.digest().toString('hex'));

const blake2bp = addon.createHash('blake2bp');
blake2bp.update(new Buffer('blake2bp', 'utf-8'));
console.log('0x' + blake2bp.digest().toString('hex'));
