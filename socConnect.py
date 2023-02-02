import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('192.168.2.220', 8080)
#print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

