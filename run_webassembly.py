#!/usr/bin/python3

# Build for Emscripten, then run this script.
# cd emsdk
# ./emsdk activate latest
# . emsdk_env.sh
# cd breakout
# mkdir build
# cd build
# emcmake cmake -GNinja ..
# emmake ninja
# ../run_webassembly.py

from http.server import SimpleHTTPRequestHandler, HTTPServer

PORT = 8000

class Handler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()

httpd = HTTPServer(("", PORT), Handler)
print(f"Serving at http://localhost:{PORT}")
httpd.serve_forever()


