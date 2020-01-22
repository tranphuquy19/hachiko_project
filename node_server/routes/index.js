var express = require('express');
var router = express.Router();
var config = require('../configs');
var sunCalc = require('../utils/suncalc');

/* GET home page. */
router.get('/', function (req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get(`/${config.server.mcuRoute}`, (req, res, next) => {
  if (sunCalc().isNight) res.status(200).send('0');
  else res.status(200).send('1');
});

router.post(`/${config.server.mcuRoute}`, (req, res, next) => {
  console.log(req.body);
  res.status(200).send('OK');
});

router.get('/suncalc', (req, res, next) => {
  res.status(200).json(sunCalc(req.query.add));
});
module.exports = router;
