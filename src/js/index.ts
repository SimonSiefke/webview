#!/usr/local/bin/node
import bindings from 'bindings'
const cpp = bindings('webview');

interface Options {
  title: string;
  url: string;
  width: number;
  height: number;
  resizable: boolean;
  debug: boolean;
}

export class WebView implements Options {
  private wvs = new cpp.WebView();

  constructor(options: Partial<Options>) {
    Object.assign(this, options);
  }

  get title() { return this.wvs.getTitle(); }
  set title(value: string) { this.wvs.setTitle(value); }
  get url() { return this.wvs.getURL(); }
  set url(value: string) { this.wvs.setURL(value); }
  get width(): number { return this.wvs.getWidth() }
  set width(value: number) { this.wvs.setWidth(value) }
  get height(): number { return this.wvs.getHeight() }
  set height(value: number) { this.wvs.setHeight(value) }
  get resizable(): boolean { return this.wvs.getResizable() }
  set resizable(value: boolean) { this.wvs.setResizable(value) }
  get debug(): boolean { return this.wvs.getDebug() }
  set debug(value: boolean) { this.wvs.setDebug(value) }

  init() {
    this.wvs.init();
    const next = () => {
      this.wvs.loop();
      // Use microtask to ensure not to block the main thread.
      // Still figuring out what the best interval is.
      setTimeout(next, 10);
    };
    next();
  }

  terminate() {
    return this.wvs.terminate();
  }
  eval(javascript: string) {
    return this.wvs.eval(javascript);
  }
  injectCSS(css: string) {
    return this.wvs.injectCSS(css);
  }
  exit() {
    return this.wvs.exit();
  }
  // debug() { return bridge.debug(this); }
  printLog(log: string) {
    return cpp.printLog(log);
  }
}

process.on('message', (value) => {

});