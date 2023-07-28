import socket as sk
from time import sleep

with sk.socket(sk.AF_INET, sk.SOCK_STREAM) as s:
    s.connect(('cs.muic.mahidol.ac.th', 80))

    ## at this point, it's connected!
    s.sendall(b"GE")
    sleep(1)
    s.sendall(b"T / HTTP/1.1")
    sleep(0.5)
    s.sendall(b"\r")
    sleep(0.5)
    s.sendall(b"\n")
    s.sendall(b"Host: cs.muic.mahidol.ac.th\r\n")
    sleep(0.5)
    s.sendall(b"Connection: close\r\n\r\n")

    while data := s.recv(1024):
        print(data)
