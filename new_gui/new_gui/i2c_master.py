#new file
from smbus import SMBus
from time import sleep
 
addr_list = [0x8, 0x9] # adresses des temoins
bus = SMBus(1) # indicates /dev/ic2-1
 
print (F"Start!")
limit = 0
while limit < 8:
    for addr in addr_list:
        print(F"{addr}")
        bus.write_byte(addr, 0x1)
        sleep(1)
        bus.write_byte(addr, 0x0)
        sleep(1)
    limit += 1

