from msgtools.lib.messaging import Messaging as M
from msgtools.lib.message import Message as Msg
from msgtools.console.client import Client

M.LoadAllMessages()
cxn = Client('example')

while True:
    msg = cxn.recv()
    if msg == None:
        print("Didn't get a message, timeout occured!")
    else:
        print(msg.toJson())
    tc3 = M.Messages.TestCase3()
    tc3.SetLatitude(1.0)
    cxn.send(tc3)