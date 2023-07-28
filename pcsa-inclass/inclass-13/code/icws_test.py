import http.client

class Request:

    def __init__(self, method: str, path: str, headers: dict):
        self.method = method
        self.path = path
        self.headers = headers

def test_request(host: str, port: str, request: Request):
    conn = http.client.HTTPConnection(host = host, port = port)
    conn.set_debuglevel(1)
    conn.request(
        request.method, 
        request.path, 
        headers = request.headers
    )
    resp = conn.getresponse()
    print('-----------------------')
    print(resp.status, resp.reason)
    print(resp.headers)
    print(resp.read())

def test_requests(host: str, port: str, requests: list):
    for request in requests:
        try:
            test_request(host, port, request)
        except Exception as e:
            print(f"illegal request: {e}")

HOST = "localhost"
PORT = 22702

legit_requests = [
    Request("GET", "/", {}),
    Request("GET", "/index.html", {}),
    Request("GET", "/", {"Host" : HOST}),
    Request("GET", "/index.html", {"Host" : HOST}),
    Request("HEAD", "/", {}),
    Request("HEAD", "/index.html", {}),
    Request("HEAD", "/", {"Host" : HOST}),
]

illegal_requests = [
    Request("GET", "/invalid path", {}),
    Request("GET", "/valid_path\r\n", {}),
    Request("GET", "/", {"Host" : f"{HOST}\r"}),
    Request("GET\n", "/", {}),
    Request("HEAD", "/", {":" : ":"}),
    Request("HEAD ", "/", {"" : ""}),
    Request("HEAD", "/\n", {}),
]

test_requests(HOST, PORT, legit_requests)
test_requests(HOST, PORT, illegal_requests)
