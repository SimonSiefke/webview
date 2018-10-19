#!/usr/local/bin/node
import bindings from 'bindings'
const cpp = bindings('webview');

export class WebView {
  private wvs = new cpp.WebView();

  constructor(options: Partial<WebView>) {
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

  /** THIS IS BLOCKING. Execute this in a Worker instead. */
  init() { return cpp.init(this.wvs); }

  setTitle(title: string) {
    return cpp.setTitle(this.wvs, title);
  }
  terminate() {
    return cpp.terminate(this.wvs);
  }
  eval(javascript: string) {
    return cpp.eval(this.wvs, javascript);
  }
  injectCSS(css: string) {
    return cpp.injectCSS(this.wvs, css);
  }
  exit() {
    return cpp.exit(this.wvs);
  }
  // debug() { return bridge.debug(this); }
  printLog() {
    return cpp.printLog(this.wvs);
  }
}
