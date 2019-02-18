#!/usr/local/bin/node
"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const bindings_1 = __importDefault(require("bindings"));
const cpp = bindings_1.default('webview');
function WebView(options) {
    const wvs = new cpp.WebView(options);
    wvs.show = function show() {
        this.init();
        const next = () => {
            this.poke();
            // Use microtask to ensure not to block the main thread.
            setTimeout(next, 1);
        };
        next();
    };
    return wvs;
}
exports.WebView = WebView;
