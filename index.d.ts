#!/usr/local/bin/node
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
export declare function WebView(options: Partial<Options>): WebViewJS;
export {};
