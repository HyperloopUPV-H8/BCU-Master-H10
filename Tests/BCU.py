import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "VirtualMCU", "src"))


import src.vmcu.assertions as assertions
import src.vmcu.services.communications.FDCAN as FDCAN
import src.vmcu.pin.pinout as Pinout
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
        
    def __init__(self,high:Pinout,precharge: Pinout,d_sup: Pinout):
        self.shm = SharedMemory("gpio_BCU","state_machine_BCU")
        self.contactor = self.ContactorTest(high,precharge,d_sup)
        self.CAN = self.FDCAN_test(self.shm, Pinout.PB11, Pinout.PB10)
        self.GUI = self.Ethernet_tests("localhost", 50400, "localhost", 50500)

        
    def is_state(self,state:int):
            current_state = self.shm.get_state_machine_state(0)
            current_state = self.shm.get_state_machine_state(0)
            return current_state == state
        
        
    class ContactorTest:
        def __init__(self,high:Pinout,precharge: Pinout,d_sup: Pinout):
            self.high = DigitalOutService(self.shm, high)
            self.precharge = DigitalOutService(self.shm, precharge)
            self.d_sup = DigitalOutService(self.shm, d_sup)
        
        def check_open_contactors(self):
            return self.high.get_pin_state() == 0 and self.precharge.get_pin_state() == 0 and self.d_sup.get_pin_state() == 0
        
        def check_precharging_contactors(self):
            return self.high.get_pin_state() == 0 and self.precharge.get_pin_state() == 1 and self.d_sup.get_pin_state() == 1
        
        def check_charging_contactors(self):
            return self.high.get_pin_state() == 0 and self.precharge.get_pin_state() == 0 and self.d_sup.get_pin_state() == 1
        
        def check_close_contactors(self):
            return self.high.get_pin_state() == 1 and self.precharge.get_pin_state() == 0 and self.d_sup.get_pin_state() == 1
        
        def check_open(self):
            assertions.check(self.check_open_contactors, msg="Contactors are not closed while in open state")
        
        def completes_open(self,before: float|None=None,after:float|None=None):
            assertions.completes(assertions.wait_until_true(self.check_open_contactors), before=before,after=after, msg="Contactors are not closed while in open state")
        
        def check_precharging(self):
            assertions.check(self.check_precharging_contactors, msg="Contactors are not open while in discharging state")
        
        def completes_precharging(self,before: float|None=None,after:float|None=None):
            assertions.completes(assertions.wait_until_true(self.check_precharging_contactors), before=before,after=after, msg="Contactors are not open while in discharging state")
        
        def check_close(self):
            assertions.check(self.check_close_contactors,msg="Contactors are not closed while in close state")
        
        def completes_close(self,before: float|None=None,after:float|None=None):
            assertions.completes(assertions.wait_until_true(self.check_close_contactors), before=before,after=after, msg="Contactors are not closed while in close state")
        
        def check_charging(self):
            assertions.check(self.check_charging_contactors,msg="Contactors are not open while in charging state")
        
        def completes_charging(self,before: float|None=None,after:float|None=None):
            assertions.completes(assertions.wait_until_true(self.check_charging_contactors), before=before,after=after, msg="Contactors are not open while in charging state")
        
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

        def receive_test(self):
            def PacketReceived():
                packet = self.fdcan.read()
                if packet != None: #TODO: packet not defined
                    return True
                return False
            assertions.completes(assertions.wait_until_true(PacketReceived), before=assertions.seconds(1), msg="FDCAN did not receive the expected packet")
                        
        def send_data_supercaps(self):
            self.receivettest()
            for i in {0,2}:
                for j in {1,4}:
                    assertions.completes(assertions.wait_until_true(self.fdcan.transmit(i,self.fdcanData[i][j],FDCAN.DLC.BYTES_8)), before=assertions.seconds(1), msg="FDCAN did not transmit")
                    assertions.completes(assertions.wait_until_true(self.fdcan.transmit(i,self.fdcanData[i][j],FDCAN.DLC.BYTES_8)), before=assertions.seconds(1), msg="FDCAN did not transmit")
        
        def reset_supercaps(self):
            self.receivettest()
            for i in {0,2}:
                assertions.completes(assertions.wait_until_true(self.fdcan.transmit(i,self.fdcanData[i][7],FDCAN.DLC.BYTES_8)), before=assertions.seconds(1), msg="FDCAN did not transmit")
            
    class SPI_tests:#TODO
        def __init__(self):
            ...
    class Ethernet_tests:
        def __init__(self,lip, lport, rip, rport):
            self.sock = Socket(lip, lport, rip, rport)
            packet_definition= {
                900:[],
                901:[]
            }
            packets= Packets(packet_definition)
            self._opencontactors_Packet= packets.serialize_packets(900)
            self._closecontactors_Packet= packets.serialize_packets(901)
        
        def connect_gui(self):
            self.sock.connect()
        
        def disconnect_gui(self):
            self.sock.stop()
            
        def transmit_open_contactors(self)->bool:
            self.sock.transmit(self._opencontactors_Packet)
            return self.sock.is_running()

        def transmit_close_contactors(self)->bool:
            self.sock.transmit(self._closecontactors_Packet)
            return self.sock.is_running()
        
        def check_packet(self)->bool: #def check_packet(self,id,value)->bool:
            def check_received_packet():#def check_received_packet(id,value):
                raw_data = self.sock.get_packet()
                if raw_data is None: 
                    return False
            #id,value = struct.unpack("<Hi",raw_data) used in test from template_project
            #return id == 1 and value == i + 5
                return True
            assertions.completes(assertions.wait_until_true(check_received_packet), before=assertions.seconds(1), msg="Packet not received")           
        
        def __del__(self):
            self.sock.stop()
            