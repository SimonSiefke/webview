#!/bin/bash
PATH=$PATH:$(npm bin)

node-gyp configure build
tsc --esModuleInterop -t ES6 --module CommonJS --outDir lib src/js/index.ts
tsc --emitDeclarationOnly -d && mv src/js/*.d.ts .