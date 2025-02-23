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
    bcu = BCU.BCU()
    
    #bcu is currently in connecting state:
    def Check_connecting():
        return bcu.is_state(BCU.BCU.General_SM.CONNECTING)
    assertions.completes(assertions.wait_until_true(Check_connecting), msg="bcu is not in connecting state")
    
    print("f")
    #bcu.contactor.check_open()  not working for now
    #Start recieving data from the supercaps CAN: Not working for now
    #bcu.CAN.start("127.0.0.1",7070,6969)
    #bcu.CAN.receive_test()
    #bcu.CAN.send_data_supercaps()
    
    
    #bcu transition to operational:
    print("g")
    bcu.GUI.connect_gui()
    print("h")
    def Check_operational():
        return bcu.is_state(BCU.BCU.General_SM.OPERATIONAL)
    assertions.completes(assertions.wait_until_true(Check_operational), msg="bcu is not in operational state")
    #Close contactors:
    print("j")
    bcu.GUI.transmit_close_contactors()
    print("k")
    bcu.contactor.completes_precharging(before = assertions.milliseconds(500))
    print("l")
    #bcu.contactor.completes_close(before = assertions.seconds(5.5),after= assertions.seconds(5))
    
    #Open contactors:
    print("m")
    bcu.GUI.transmit_open_contactors()
    bcu.contactor.completes_open(before = assertions.milliseconds(500))
    
    #bcu transition to fault:
    print("n")
    bcu.GUI.disconnect_gui()
    print("o")
    time.sleep(1)
    def Check_fault():
        return bcu.is_state(BCU.BCU.General_SM.FAULT)
    
    assertions.completes(assertions.wait_until_true(Check_fault),msg="bcu is not in fault state")    
    
    

runner.run() # Runs the tests, do not delete!
