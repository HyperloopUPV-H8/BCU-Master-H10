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
    bcu = BCU.BCU(Pinout.PE6, Pinout.PF1, Pinout.PF2)
    
    #bcu is currently in connecting state:
    def funcion_prueba():
        return bcu.is_state(BCU.BCU.General_SM.CONNECTING)
    assertions.completes(assertions.wait_until_true(funcion_prueba), msg="bcu is not in connecting state")
    
    print("f")
    bcu.contactor.check_open()
    #Start recieving data from the supercaps CAN:
    #bcu.CAN.start(127.0.0.1,7070,6969)
    #bcu.CAN.receive_test()
    #bcu.CAN.send_data_supercaps()
    
    
    #bcu transition to operational:
    print("g")
    bcu.GUI.connect_gui()
    print("h")
    assertions.completes(assertions.wait_until_true(bcu.is_state),args=(bcu.General_SM.OPERATIONAL),before=(assertions.milliseconds(500)), msg="bcu is not in operational state")
    print("i")
    bcu.contactor.check_open()
    
    #bcu operational:
    
    #Open contactors:
    print("j")
    bcu.GUI.transmit_close_contactors()
    print("k")
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
