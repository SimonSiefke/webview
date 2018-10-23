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

interface WebViewJS extends Options {
  terminate(): void;
  eval(javascript: string): void;
  injectCSS(css: string): void;
  exit(): void;
  setFullscreen(fullscreen: boolean): void;
  show(): void;
}

export function WebView(options: Partial<Options>) {
  const wvs: WebViewJS = new cpp.WebView(options);
  wvs.show = function show() {
    (this as any).init();
    const next = () => {
      this.loop();
      // Use microtask to ensure not to block the main thread.
      // Still figuring out what the best interval is.
      setTimeout(next, 10);
    };
    next();
  }
  return wvs;
}
