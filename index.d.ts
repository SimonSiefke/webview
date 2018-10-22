#!/usr/local/bin/node
interface Options {
    title: string;
    url: string;
    width: number;
    height: number;
    resizable: boolean;
    debug: boolean;
}
export declare class WebView implements Options {
    private wvs;
    constructor(options: Partial<Options>);
    title: string;
    url: string;
    width: number;
    height: number;
    resizable: boolean;
    debug: boolean;
    init(): void;
    terminate(): any;
    eval(javascript: string): any;
    injectCSS(css: string): any;
    exit(): any;
    printLog(log: string): any;
}
export {};
