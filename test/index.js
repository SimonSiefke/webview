#!/usr/local/bin/node
const { WebView } = require('../');
const express = require('express');

const app = express();
app.use(express.static(__dirname));
app.listen(3000);

const view = WebView({
  title: 'Accueil',
  url: `http://localhost:3000/index.html`,
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
