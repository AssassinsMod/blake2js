#!/usr/bin/env node

'use strict';

const api = require('bindings')('hook');

console.log(api.Blake2bInit());
