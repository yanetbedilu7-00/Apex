from http.server import BaseHTTPRequestHandler, HTTPServer

class C2Handler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        
        print("\n[!] INCOMING BEACON FROM IMPLANT!")
        print(f"[+] Data received: {post_data.decode('utf-8', errors='ignore')}")
        
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"OK")

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"C2 Server Running")

if __name__ == '__main__':
    server = HTTPServer(('127.0.0.1', 8080), C2Handler)
    print("[*] C2 Server listening on http://127.0.0.1:8080")
    server.serve_forever()
