{
  "targets": [
    {
      "target_name": "webview",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "webview.cc" ],
      'libraries': [
        '<!@(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0)',
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        # '<!@(pkg-config --cflags-only-I gtk+-3.0 webkit2gtk-4.0)',

        "/usr/include/webkitgtk-4.0",
        "/usr/include/gtk-3.0",
        "/usr/include/at-spi2-atk/2.0",
        "/usr/include/at-spi-2.0",
        "/usr/include/dbus-1.0",
        "/usr/lib/x86_64-linux-gnu/dbus-1.0/include",
        "/usr/include/gtk-3.0",
        "/usr/include/gio-unix-2.0",
        "/usr/include/cairo",
        "/usr/include/pango-1.0",
        "/usr/include/harfbuzz",
        "/usr/include/pango-1.0",
        "/usr/include/fribidi",
        "/usr/include/harfbuzz",
        "/usr/include/atk-1.0",
        "/usr/include/cairo",
        "/usr/include/pixman-1",
        "/usr/include/uuid",
        "/usr/include/freetype2 -I/usr/include/libpng16",
        "/usr/include/gdk-pixbuf-2.0",
        "/usr/include/libsoup-2.4",
        "/usr/include/libxml2",
        "/usr/include/libmount",
        "/usr/include/blkid",
        "/usr/include/webkitgtk-4.0",
        "/usr/include/glib-2.0",
        "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
