#!/usr/local/bin/node
const { WebView } = require('../')

const view = new WebView({
  title: 'Wikipedia',
  url: 'https://en.wikipedia.org',
  height: 800,
  width: 600,
});

view.init();
