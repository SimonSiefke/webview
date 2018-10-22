#!/usr/local/bin/node
const { WebView } = require('../')

const view = new WebView({
  title: 'Wikipedia',
  url: 'https://www.youtube.com',
  height: 800,
  width: 600,
});

view.init();
console.log('Vous devez voir une page de YouTube.');

setTimeout(() => {
  console.log('Test. Task not blocked.')
}, 500);