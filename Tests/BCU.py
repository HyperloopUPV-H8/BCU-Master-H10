import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "VirtualMCU", "src"))


import src.vmcu.assertions as assertions
import src.vmcu.services.communications.FDCAN as FDCAN
import src.vmcu.pin.pinout as Pinout
import src.vmcu.services.digital_in as DigitalInService
import src.vmcu.services.digital_out as DigitalOutService
import src.vmcu.shared_memory as SharedMemory
import src.vmcu.services.communications.spi as SPI
import src.vmcu.services.communications.Socket as Socket
import src.vmcu.services.communications.Packets as Packets
from enum import Enum

class BCU():
    
    class General_SM(Enum):
        CONNECTING = 0
        OPERATIONAL = 1
        FAULT = 2
        
    def __init__(self):
        self.shm = SharedMemory("gpio__blinking__led","state_machine__blinking__led")
        
        
    def is_state(self,state:int):
            current_state = self.shm.get_state_machine_state(1)
    class ContactorTest:
        def __init__(self,high:Pinout,precharge: Pinout,d_sup: Pinout):
            self.high = DigitalOutService(self.shm, high)
            self.precharge = DigitalOutService(self.shm, precharge)
            self.d_sup = DigitalInService(self.shm, d_sup)
            
        def check_charging(self):
            def check_charging_contactors():
                if self.high.get_pin_state() == 1:
                    return False
                if self.precharge.get_pin_state() == 1:
                    return False
                if self.d_sup.get_pin_state() == 1:
                    return False
                return True
            assertions.completes(assertions.wait_until_true(check_charging_contactors), before=assertions.seconds(1), msg="Contactors are not closed while in charging state")
        
        def check_open(self):
            def check_open_contactors():
                if self.high.get_pin_state() == 1:
                    return False
                if self.precharge.get_pin_state() == 1:
                    return False
                if self.d_sup.get_pin_state() == 0:
                    return False
                return True
            assertions.completes(assertions.wait_until_true(check_open_contactors), before=assertions.seconds(1), msg="Contactors are not open while in open state")
            
        
        def check_close(self):
            def check_close_contactors():
                if self.high.get_pin_state() == 0:
                    return False
                if self.precharge.get_pin_state() == 0:
                    return False
                if self.d_sup.get_pin_state() == 1:
                    return False
                return True
            assertions.completes(assertions.wait_until_true(check_close_contactors), before=assertions.seconds(1), msg="Contactors are not closed while in close state")
        
    class FDCAN_test:
        def __init__(self,shm:SharedMemory, Rx: Pinout, Tx: Pinout):
            self.fdcan = FDCAN(Rx, Tx, shm)
            self.fdcanids ={}
            self.fdcanData={}
            for i in {0,2}:
                for j in {1,4}:
                    self.fdcanids[i][j] =j*256+16+i
                self.fdcanids[i][7] = 7*256+16+i
                self.fdcanData[i][1] = bytearray([0x00, 0x01, 0x88, 0x88, 100,69,0,0])
                self.fdcanData[i][2] = bytearray([0x88,0x88,0x69,0x69,0x78,0x78,0x0F,0x0A])
                self.fdcanData[i][3] = bytearray([None,None,0,0,0,0,0,0])
                self.fdcanData[i][4] = bytearray([0,1,0x88,0x88,0x69,0x69,0x78,0x78])                
                self.fdcanData[i][7] = bytearray([0,0,0,0,0,0,0,0])
            
            
        def start(self, ip: str, port: int,sendport:int):
            self.fdcan.start(ip, port,sendport)

        def receivettest(self):
            def PacketReceived():
                packet = self.fdcan.read()
                if packet != None: #TODO: packet not defined
                    return True
                
            assertions.completes(assertions.wait_until_true(PacketReceived), before=assertions.seconds(1), msg="FDCAN did not receive the expected packet")
        def testSupercaps(self):
            self.receivettest()
            for i in {0,2}:
                for j in {1,4}:
                    assertions.completes(self.fdcan.transmit(i,self.fdcanData[i][j],FDCAN.DLC.BYTES_8), before=assertions.seconds(1), msg="FDCAN did not transmit")
        
        def testresetSupercaps(self):
            self.receivettest()
            for i in {0,2}:
                assertions.completes(self.fdcan.transmit(i,self.fdcanData[i][7],FDCAN.DLC.BYTES_8), before=assertions.seconds(1), msg="FDCAN did not transmit")
            
    class SPI_tests:#TODO
        def __init__(self):
            ...
    class Ethernet_tests:
        def __init__(self,lip, lport, rip, rport):
            self.sock = Socket(lip, lport, rip, rport)
            packet_definition= {
                900:bytearray([0x03,0x84,...]),
                901:bytearray([0x03,0x85,...])
            }
            self.packets= Packets(packet_definition)
        
        def connect_gui(self):
            self.sock.connect()
        
        def disconnect_gui(self):
            self.sock.stop()
            
        def transmit(self,packet):
            self.sock.transmit(packet)
        
        def __del__(self):
            self.sock.stop()
            