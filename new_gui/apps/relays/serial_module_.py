#python
import os
import sys
import glob
import serial
import time
from pathlib import Path


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    #pop out default serial device
    try:
        index = result.index('/dev/ttyAMA0')
        result.pop(index)
    except:
        pass

    return result

def get_data(port_name, file_name, **kwargs):
    print("Connecting to serial device")
    temoin = serial.Serial(
            port_name,
            9600,
            timeout=.1
            )
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    file_path = os.path.join(
            BASE_DIR,
            F"static/data/{file_name}.csv"
           )
    print(file_path)
    print("esperalooo")
    time.sleep(4)
    print("sale\n")


    file = open(file_path, "a")

    temoin.write(b'start\n')
    incomming_data = b''
    llega = b''
    try:
        while not incomming_data == b'!':
            incomming_data = temoin.read()
            if( not(#si cest pas \n ni \r
                incomming_data == b'\n'
                or
                incomming_data == b'\r'
                )
            ):
                llega += incomming_data

            if(incomming_data == b'\n'):
                #print(F'==> {ascii(llega.decode())}')
                file.write(F"{ascii(llega.decode())}\n")
                llega = b""
                incomming_data = b''
    except:
        pass


    file.close()

    temoin.close()


if __name__ == '__main__':
    get_data()

