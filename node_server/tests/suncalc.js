var SunCalc = require("suncalc");
var momentTz = require('moment-timezone');
var times = SunCalc.getTimes(new Date(), 51.5, -0.1);
console.log(`${momentTz.tz(times.sunset, `Europe/London`).format(`DD/MM/YYYY HH:mm:ss`)}`);