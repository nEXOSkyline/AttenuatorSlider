import tkinter as tk
import socket, threading
import time
import datetime
import sys
import os
import random
from pymodbus.client import ModbusTcpClient
import struct

def data_to_float32(data) :
    #return struct.unpack('<f',struct.pack('=I',concatData(data)))[0]
    return concatData(data)
def concatData (data):
    msb=struct.pack('=I',data[0])
    lsb=struct.pack('=I',data[1]<<16)
    return struct.unpack('<f',msb[0:2] + lsb[2:4])
#int(bytearray((msb[0:2]+lsb[2:4]).encode()))

class slidegui(tk.Frame) :
    def on_closing(self) :
        self.run = False
        os._exit(0)
    def tare(self):
        if self.coil_state == 0x00 :
          self.coil_state = 0xff
        else :
          self.coil_state = 0x00
        self.mbc.write_coil(0x0a,self.coil_state, unit=1)
        #self.read_mass()
    def set_position(self,val) :
        pos = int(val)
        print(pos)
        z_bytes = struct.pack('<I',pos)
        print(z_bytes)
        self.mbc.write_register(0x00,z_bytes,unit=1)
        #msg = chr(0x00) + chr(0x00) + chr(0x00) + chr(0x00) + chr(0x00) + chr(0x0a) + chr(0x00)#trans protocol length unit
        #msg = msg + chr(0x10) + chr(0x00) + chr(0x04) + chr(0x00) + chr(0x02) + chr(z_bytes[0]) + chr(z_bytes[1]) + chr(z_bytes[2]) + chr(z_bytes[3])
        #self.ser.send( msg.encode('iso-8859-1') )
        #response = self.ser.recv(10).decode('iso-8859-1')#comment out these lines later

    def keepAlive(self):   #runs a while loop "forever" and it calls read_holding_register() (read the coil state) and read it every 
        #self.mbc.read_holding_registers(0,2,unit=1)
        self.mbc.write_coil(0x0c,0xff, unit=1)
        self.parent.after(1000,self.keepAlive)
  

        
    def __init__(self,parent):
        tk.Frame.__init__(self)
        #self.window = Tk()
        #self.window.geometry('360x128')
        #self.window.title( 'slider' )
        self.parent = parent
        #self.ser = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
        #self.ser.connect(('localhost',502))
        self.mbc = ModbusTcpClient('10.0.0.4')
        self.mbc.connect()

class slider(tk.Tk) :
    def __init__(self):
        tk.Tk.__init__(self)

        # Set title and screen resolutions
        tk.Tk.wm_title(self, 'Slider')
        tk.Tk.minsize(self, width=360, height=80)

        # Show window and control bar
        self.slide = slidegui(self)
        #tk.Label(self, text='calibration factor').grid(row=0)
        #tk.Label(self, text='kg').grid(row=2)
        
        #self.cal_entry = tk.Entry(self)
        #self.z_entry = tk.Entry(self)
        #self.mass_disp = tk.Text(self,height=1,width=20)

        #self.cal_entry.grid(row=0,column=1)
        #self.mass_disp.grid(row=2,column=1)

        self.coil_state = 0x00

        #tk.Button(self,text='Send',command=self.slide.set_zero).grid(row=0,column=2)
        #tk.Button(self,text='TARE',command=self.slide.tare).grid(row=3,column=1)
        self.scale = tk.Scale(self, from_=0, to=50000, length=200,orient=tk.HORIZONTAL,command=self.slide.set_position)
        self.scale.grid(row=2,column=1)
        #self.slide.keepAlive()

appl = slider()
appl.mainloop()
   
