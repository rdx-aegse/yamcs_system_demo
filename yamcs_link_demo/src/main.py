import yamcs_link
from yamcs_link.yamcs_userlib import EventSeverity, YAMCSObject, telemetry, telecommand, event, U8, U16, F32, I16
from yamcs_link.yamcs_link import YAMCS_link
from enum import Enum
from typing import override
import os
import time

MDB_NAME = "YAMCS_link_demo" #Desired name of the YAMCS mission database for this demo
MDB_VERSION = "1.0" #Unused here, needed by YAMCS mission generator. Simply shown in the MDB 
YAMCS_MDB_DIR = '/mdb_shared' #Output folder for the YAMCS mission databases

#Dummy example app definition

class MyEnum(Enum):
    VALUE1=1
    VALUE2=2

class MyComponent(YAMCSObject):
    def __init__(self, name):
        YAMCSObject.__init__(self, name)
    
    #Optional but highly recommended on_disconnect
    @override
    def on_disconnect(self):
        logging.info(f'{self.yamcs_name} would now go back to safe state after YAMCS disconnected')

    @telemetry(1000) #milliseconds period
    def my_telemetry1(self) -> MyEnum:
        return MyEnum.VALUE1.value

    @telemetry(2000) #milliseconds period
    def my_telemetry2(self) -> U8:
        return 42

    @telecommand(arg1=[5,10], arg3=[None, 1000])
    def my_command(self, arg1: U16, arg2: I16, arg3: F32) -> U8:
        logging.info(f'MyComponent.my_command was invoked on {self.yamcs_name} with args {arg1}, {arg2}, {arg3}')
        logging.info(f'Triggering event my_event')
        self.my_event(arg1, arg2)
        return 0
    
    @event(EventSeverity.INFO)
    def my_event(self, arg1 : U16, arg2 : F32) -> str:
        return f'Dummy event triggered with arguments arg1={arg1} and arg2={arg2}'
    
#initialisation
yamcs_link = YAMCS_link("my_link", tcp_port=int(os.environ.get('PORT_TC')), udp_port=int(os.environ.get('PORT_TM'))) 
my_component = MyComponent("component1")
yamcs_link.register_yamcs_child(my_component)

#Generate mdb is necessary for YAMCS to know how to interact with the app. 
# Make sure there is an automated process for yamcs to start up from those updated mdb
yamcs_link.generate_mdb(YAMCS_MDB_DIR, MDB_NAME, MDB_VERSION) 
#If the mdb is generated through some other scheme, manually do call update_index() between register_yamcs_child() and service()

# Main loop
try:
    while True:
        #Possible to do things here e.g. if your app doesn't inherit from YAMCS_link
        yamcs_link.service() #Send due TM and process pending command then return
        time.sleep(0.1) #Small delay to prevent busy-waiting
except KeyboardInterrupt:
    logging.info("Exiting main loop.")
finally:
    yamcs_link._shutdown() 