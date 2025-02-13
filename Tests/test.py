import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "VirtualMCU", "src"))

from runner import runner
from src.vmcu.shared_memory import SharedMemory
from src.vmcu.pin import Pinout, DigitalOut
from src.vmcu.services.digital_out import DigitalOutService
import src.vmcu.assertions as assertions
import BCU as BCU
import time


@runner.test()
def BCU_test():
    #Initialize the BCU test class
    BCU = BCU(Pinout.PA1, Pinout.PA2, Pinout.PA3)
    
    #BCU is currently in connecting state:
    assertions.check(BCU.is_state,args=(BCU.General_SM.CONNECTING), msg="BCU is not in connecting state")
    BCU.contactor.check_charging()
    #Start recieving data from the supercaps CAN:
    BCU.CAN.start()
    BCU.CAN.receive_test()
    time.sleep(0.5)
    BCU.CAN.send_data_supercaps()
    
    
    #BCU transition to operational:
    BCU.GUI.connect_gui()
    time.sleep(0.5)
    assertions.check(BCU.is_state,args=(BCU.General_SM.OPERATIONAL), msg="BCU is not in operational state")
    
    #BCU operational:
    
    #Open contactors:
    BCU.GUI.transmit_open_contactors()
    time.sleep(0.5)
    BCU.contactor.check_open()
    
    #Close contactors:
    BCU.GUI.transmit_close_contactors()
    time.sleep(0.5)
    BCU.contactor.check_charging()
    
    #BCU transition to fault:
    BCU.CAN.disconnect_gui()
    time.sleep(0.5)
    assertions.check(BCU.is_state,args=(BCU.General_SM.FAULT), msg="BCU is not in fault state")
    
    
    

runner.run() # Runs the tests, do not delete!
