norns2sinfonion allows [mosaic](https://github.com/subvertnormality/mosaic) to sync it's global scales with the Sinfonion eurorack quantiser. It may allow other norns scripts to do the same with a bit of work (interested? feel free to (message me)[https://llllllll.co/u/byzero/]). 

To enable this, you need to do a bit of DIY but it's about as straight forward as it gets. 

You'll need:

* A Teensy 3.2+
* A 47 Ω resister
* A TS Jack
* The ability to hook this together, probably by soldering onto strip board

You need to solder the TS Jack's tip connector to a 47 Ω resister and then on to the TX2 pin on your teensy. The sleeve connector needs to be soldered onto the ground pin on the teensy. Flash your teensy with the included firmware. Select "MIDI" from the Tools->USB Type menu of the arduino app before uploading flashing.

You should end up with something that looks a bit like this:

![A built norns2sinfonion](https://github.com/subvertnormality/norns2sinfonion/blob/main/images/norns2sinfonion.jpg?raw=true)

The TS Jack connects to one of your Sinfonion in ports. Select this port to be sync in from the options menu.