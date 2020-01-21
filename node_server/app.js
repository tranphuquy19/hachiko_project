var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var logger = require('morgan');
var webPush = require('web-push');
var i18n = require('i18n');

const { server } = require('./configs');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

// i18n config
i18n.configure({
  locales: ['vi', 'en', 'ja', 'zh'],
  defaultLocale: 'vi',
  queryParameter: 'lang',
  directory: path.join(__dirname, 'public', 'langs'),
  cookie: 'lang'
});
app.use(i18n.init);

//config web push
webPush.setVapidDetails(`mailto:${server.email}`, server.pubVapidKey, server.priVapidKey);

app.use('/', indexRouter);
app.use('/users', usersRouter);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  next(createError(404));
});

// error handler
app.use(function (err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
