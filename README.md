# WebHandVrDriver

WebHandVrDriver - a SteamVR driver that can communicate to custom hardware over the UDP protocol. It uses the Vive Wand bindings, so it has everything You need.

## For devs:
**How do I use this driver for my custom controller?**
You need to implement a straight UDP communication to Your computer, the default port for the driver is 49152. The message syntax, sent every frame should be: **PosX1|PosY1|PosZ1|RotX1|RotY1|RotZ1|PosX2|PosY2|PosZ2|RotX2|RotY2|RotZ2|A|B|X|Y|Joy1X|Joy1Y|Joy2X|Joy2Y|Trig1|Trig2|Grip1|Grip2|AppMenu1|AppMenu2|Joy1Click|Joy2Click|System1|System2**. ABXY will be implemented soon, but You need them in the message. The driver then reads this and sends info to SteamVR.
