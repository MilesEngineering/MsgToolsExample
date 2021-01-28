addpath("../../MsgTools/MsgApp")

m = Messaging("../obj/CodeGenerator/Matlab");
c = MessageClient(m,Messages.headers.NetworkHeader);

msg = Messages.TestCase4();
msg.A = 12;
c.SendMsg(msg);

c.SubscribeToMessage(Messages.TestCase2.MSG_ID);
fprintf("Subscribing to %d (0x%X)\n", Messages.TestCase2.MSG_ID, Messages.TestCase2.MSG_ID);

while true
    msg = c.GetMsg();
    display(msg);
end
