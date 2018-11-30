#!/usr/local/bin/node
const { WebView } = require('../')

const view = WebView({
  title: 'Google',
  url: 'https://www.google.com',
  height: 600,
  width: 800,
});

view.show();
console.log(view.url);
console.log('Vous devez voir une page de Google.');

const interval = 500;
const now = Date.now();
setTimeout(() => {
  console.log('Test. Task not blocked.');
  console.log(`Expected: ${interval}, Actual: ${Date.now() - now}`);
  // view.setFullscreen(true);
}, interval);