from machine import Pin
from onewire import DS18X20
from onewire import OneWire
from network import WLAN, Sigfox
import pycom
import binascii
import time
import socket
import machine

# init Sigfox for RCZ1 (Europe)
sigfox = Sigfox(mode=Sigfox.SIGFOX, rcz=Sigfox.RCZ1)

# create a Sigfox socket
s = socket.socket(socket.AF_SIGFOX, socket.SOCK_RAW)

print('I am device ',  binascii.hexlify(sigfox.id()) )
 # make the socket blocking
s.setblocking(True)

# configure it as uplink only
s.setsockopt(socket.SOL_SIGFOX, socket.SO_RX, False)

pycom.heartbeat(False)
ow = OneWire(Pin('G17'))
temp = DS18X20(ow)


temp.start_convertion()
time.sleep(1)
result = temp.read_temp_async()
wlan = WLAN(mode=WLAN.STA)
nets = wlan.scan()
nets = sorted(nets,  key=lambda x:x.rssi,  reverse=True)
wifi = []

print(str(result))
pycom.rgbled(0x003300)
s.send(str(result))
pycom.rgbled(0x000000)

if len(nets) >= 2:
    wifi = nets[0].bssid + nets[1].bssid
    print(wifi)
    pycom.rgbled(0x003300)
    s.send(wifi.decode())
    pycom.rgbled(0x000000)

machine.deepsleep(60*1000*10)
