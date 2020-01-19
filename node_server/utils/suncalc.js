var { homeConfig } = require('../configs');
var SunCalc = require('suncalc');
var moment = require('moment');
var momentTz = require('moment-timezone');

const { latitude, longitude, timezone, timeFormat } = homeConfig;

function format(times) {
    return `${momentTz.tz(times, `${timezone}`).format(`${timeFormat}`)}`
}

function calc(addHours = 0) {
    var now = new moment().add(addHours, 'hours');

    const times = SunCalc.getTimes(now, latitude, longitude);
    const sunrise = times.sunriseEnd;
    const sunset = times.sunsetStart;

    return {
        timezone,
        latitude,
        longitude,
        now: format(now),
        nowUnix: Number(now),
        currentDaySunriseFormat: format(sunrise),
        currentDaySunriseUnix: Number(sunrise),
        currentDaySunsetFormat: format(sunset),
        currentDaySunsetUnix: Number(sunset),
        isNight: ((Number(now - sunrise) <= 0) || (Number(now - sunset) > 0)),
    };
}

module.exports = calc;