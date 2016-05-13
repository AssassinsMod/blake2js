#!/usr/bin/env node

'use strict';

const addon = require('bindings')('addon');

//*
const EH = require('segfault-handler');
EH.registerHandler('crash.log');
//*/

const blake2s = addon.createHash('blake2s');
blake2s.update(new Buffer('Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.'));
console.log('0x' + blake2s.digest().toString('hex'));
