{
  "targets": [
    {
      "target_name": "webview",
      "sources": [
        "src/js-types.cpp",
        "src/webview-struct.cpp",
        "src/main.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": ["NAPI_CPP_EXCEPTIONS=1"],
      "conditions": [
        ['OS=="mac"', {
          "defines": ["WEBVIEW_COCOA=1"],
          "libraries": ["-framework Webkit"],
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          }
        }],
        ['OS=="linux"', {
          "defines": ["WEBVIEW_GTK=1"],
          "libraries": ["<!@pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0)"]
        }]
      ]
    }
  ]
}
