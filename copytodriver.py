import os

file1location ="build/OpenVR/samples/driver_sample/Release/driver_webhandsvr.dll"
# file1copydestination = "Driver/WebHandsVR/bin/win64/driver_webhandsvr.dll"
# C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\webhandsvr\bin\win64\driver_webhandsvr.dll
file1copydestination = "C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr/bin/win64/driver_webhandsvr.dll"

drivermanifest="""
{
	"alwaysActivate": true,
	"name" : "webhandsvr",
	"directory" : "",
	"resourceOnly" : false,
	"hmd_presence" :
	[
		"*.*"
	]
}
"""

drivermanifestlocation = "C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr/driver.vrdrivermanifest"

steamvrsettings="""
{
   "driver_webhandsvr" : {
      "enable" : true,
      "id" : "webhandsvr",
	  "renderWidth" : 1920,
      "renderHeight" : 1080,
      "secondsFromVsyncToPhotons" : 0.10000000149011612,
	  "displayFrequency" : 60.0,
      "serialNumber" : "Null 4711",
      "windowHeight" : 1080,
      "windowWidth" : 1920,
      "windowX" : 0,
      "windowY" : 0
   },
   "steamvr" : {
      "activateMultipleDrivers" : true,
      "enableHomeApp" : false,
      "forcedDriver" : "webahndsvr",
      "mirrorViewGeometry" : "0 0 960 540"
   }
}
"""

steamvrsettingslocation = "C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr/steamvr.vrsettings"


if __name__ == "__main__":
    print("Copying files to driver...")

    #check if C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\webhandsvr exists
    if not os.path.exists("C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr"):
        print("Driver not found, creating...")
        os.makedirs("C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr")
        os.makedirs("C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr/bin")
        os.makedirs("C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers/webhandsvr/bin/win64")

        #check if manifest exists
        if not os.path.exists(drivermanifestlocation):
            print("Driver manifest not found, creating...")
            with open(drivermanifestlocation, "w") as f:
                f.write(drivermanifest)

        #check if settings exists
        if not os.path.exists(steamvrsettingslocation):
            print("SteamVR settings not found, creating...")
            with open(steamvrsettingslocation, "w") as f:
                f.write(steamvrsettings)


    os.makedirs(os.path.dirname(file1copydestination), exist_ok=True)
    file1location = os.path.abspath(file1location)
    file1copydestination = os.path.abspath(file1copydestination)
    print("Copying " + file1location + " to " + file1copydestination)
    os.system(f'copy {file1location} "{file1copydestination}"')