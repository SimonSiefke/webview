#!/usr/local/bin/node
const { WebView } = require('../')

const view = WebView({
  title: 'YouTube',
  url: 'https://www.youtube.com',
  height: 600,
  width: 800,
});

view.show();
console.log(view.url);
console.log('Vous devez voir une page de YouTube.');

setTimeout(() => {
  console.log('Test. Task not blocked.')
  // view.setFullscreen(true);
}, 500);