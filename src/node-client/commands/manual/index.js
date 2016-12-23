'use strict';

const api = require('../../api');

module.exports = function manual(level) {
  console.log('Manual control!');
  api.setMeter(level);
};
