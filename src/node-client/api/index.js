'use strict';

const axios = require('axios');
const URL_PREFIX = 'http://192.168.43.207';

module.exports = {
  setMeter
};

function setMeter(level) {
  // TODO: validation - int between 0 and 1023
  return axios.get(`${URL_PREFIX}/meter?level=${level}`);
}
