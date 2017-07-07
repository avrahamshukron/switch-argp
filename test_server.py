import sys
import socket


def main(port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", port))
    s.listen(1)
    while True:
        print "Waiting for client..."
        client, addr = s.accept()
        print "Client connected at %s" % (addr,)
        while True:
            try:
                data = client.recv(1024)
                print data
            except Exception as e:
                print str(e)
                client.close()
                print "Client closed"
                break


if __name__ == '__main__':
    port = int(sys.argv[1])
    main(port)
