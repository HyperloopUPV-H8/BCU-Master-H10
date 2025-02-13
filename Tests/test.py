import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "VirtualMCU", "src"))

from runner import runner
from vmcu.pin import Pinout
from vmcu.services.digital_out import DigitalOutService
import vmcu.assertions as assertions
import BCU as BCU
import time

    

@runner.test()
def contactors_close_test():
    #Initialize the bcu test class
    print("a")
    bcu = BCU.BCU(Pinout.PA1, Pinout.PA2, Pinout.PA3)
    print("hola")
    
    #bcu is currently in connecting state:
    assertions.check(bcu.is_state,args=(bcu.General_SM.CONNECTING), msg="bcu is not in connecting state")
    bcu.contactor.check_open()
    #Start recieving data from the supercaps CAN:
    #bcu.CAN.start(127.0.0.1,7070,6969)
    #bcu.CAN.receive_test()
    #bcu.CAN.send_data_supercaps()
    
    
    #bcu transition to operational:
    bcu.GUI.connect_gui()
    assertions.completes(assertions.wait_until_true(bcu.is_state),args=(bcu.General_SM.OPERATIONAL),before=(assertions.milliseconds(500)), msg="bcu is not in operational state")
    bcu.contactor.check_open()
    
    #bcu operational:
    
    #Open contactors:
    bcu.GUI.transmit_close_contactors()
    bcu.contactor.completes_precharging(before = assertions.milliseconds(500))
    bcu.contactor.completes_close(before = assertions.seconds(5.5),after= assertions.seconds(5))
    
    #Close contactors:
    bcu.GUI.transmit_open_contactors()
    bcu.contactor.completes_open(before = assertions.milliseconds(500))
    
    #bcu transition to fault:
    bcu.CAN.disconnect_gui()
    time.sleep(0.5)
    assertions.check(bcu.is_state,args=(bcu.General_SM.FAULT), msg="bcu is not in fault state")
    
    
    

runner.run() # Runs the tests, do not delete!
