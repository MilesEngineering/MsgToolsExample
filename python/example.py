from msgtools.lib.messaging import Messaging as M
from msgtools.lib.message import Message as Msg
from msgtools.console.client import Client

M.LoadAllMessages()
cxn = Client('ExampleClient')

while True:
    # blocking read for some number of seconds
    msg = cxn.recv(timeout=10.0)

    # check if we timeout
    if msg == None:
        print("Didn't get a message, timeout occured!")
    else:
        # print any message as JSON
        print(msg.toJson())
        # check type of specific message
        if type(msg) == M.Messages.TestCase1:
            # get a field using accessor function, or dict lookup which is equivalent
            print("tc1.FieldE: %f" % msg.GetFieldE())
            print("tc1.FieldE: %f" % msg.FieldE)
        elif type(msg) == M.Messages.TestCase2:
            print("tc2.F1: " + str(msg.F1))
    # create a message to send
    tc2 = M.Messages.TestCase2()
    # set a field
    tc2.F1 = "Val0"
    tc2.Field6 = 1
    tc2.SetF2(1.5,0)
    tc2.SetF2(2.5,1)
    tc2.SetF2(3.5,2)
    # send the message
    cxn.send(tc2)
