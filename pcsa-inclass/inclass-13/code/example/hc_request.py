import http.client

conn = http.client.HTTPConnection(host='muic.mahidol.ac.th', port=80)

conn.set_debuglevel(1)

headers = {
    'Accept': 'text/plain',
    'Blah': 'blahblah',
    'Connection': 'close',
}

conn.request('GET', '/', headers=headers)
resp = conn.getresponse()


print('-----------------------')
print(resp.status, resp.reason)
print(resp.headers)
print(resp.read())
