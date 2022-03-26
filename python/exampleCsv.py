#!/usr/bin/python3
from msgtools.lib.messaging import Messaging as M
from msgtools.console.client import Client

M.LoadAllMessages()
cxn = Client('CSV_CLI')

while True:
    msg = cxn.recv(timeout=10.0)
    if msg != None:
        print(msg.toCsv())
