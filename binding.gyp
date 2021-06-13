{
  "targets": [
    {
      "target_name": "webview",
      "cflags": [ "-fno-exceptions"],
      "cflags_cc": [ "-fno-exceptions", "-Wno-delete-non-virtual-dtor" ],
      "sources": [ "webview.cc" ],
      'libraries': [
        '<!@(pkg-config --libs-only-l gtk+-3.0 webkit2gtk-4.0)',
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(pkg-config --cflags-only-I gtk+-3.0 webkit2gtk-4.0 | sed 's/-I//g')",
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
