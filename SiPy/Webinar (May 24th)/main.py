from network import Sigfox, Wifi
import socket
import binascii
import time
from machine import Pin
from onewire import DS18X20
from onewire import OneWire

# init Sigfox for RCZ1 (Europe)
sigfox = Sigfox(mode=Sigfox.SIGFOX, rcz=Sigfox.RCZ1)

# create a Sigfox socket
s = socket.socket(socket.AF_SIGFOX, socket.SOCK_RAW)

print('I am device ',  binascii.hexlify(sigfox.id()) )
 # make the socket blocking
s.setblocking(True)

# configure it as uplink only
s.setsockopt(socket.SOL_SIGFOX, socket.SO_RX, False)

ow = OneWire(Pin('G17'))
temp = DS18X20(ow)

while True:
    temp.start_convertion()
    time.sleep(1)
    result = temp.read_temp_async()
    print(result)
    s.send(str(result))
    time.sleep(50)
