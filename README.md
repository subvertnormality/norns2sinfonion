# norns2sinfonion

norns2sinfonion allows [mosaic](https://github.com/subvertnormality/mosaic) (or another script or device) to sync it's global scales with the Sinfonion eurorack quantiser. It may allow other norns scripts to do the same with a bit of work (interested? feel free to [message me](https://llllllll.co/u/byzero/)). 

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

## General usage

norns2sinfonion can be used from any script (Norns or otherwise). It recieves midi to select the scale, root, degree, and transposition. Simply send a midi program change message to the norns2sinfonion.

The following is used in mosaic, but the modes, degrees, etc can be sent over midi by any script in theory. Note the sinf_root_mod - this is there because the sinfonion doesn't have the concept of modes. Instead mosaic uses a relative scale and sets the root note to operate at the desired mode (for example, C major with a root of D for dorian).

```
local scales = {
  {
    name = "Major",
    sinf_degrees = {0, 2, 4, 5, 7, 9, 11},
    sinf_mode = 3,
    sinf_root_mod = 0
  },
  {
    name = "Harmonic Major",
    number = 2,
    scale = musicutil.generate_scale(0, "harmonic major", 1),
    romans = {"I", "ii", "iii", "IV", "V", "♭VI+", "vii°"},
    sinf_degrees = {0, 2, 4, 5, 7, 8, 11},
    sinf_mode = 7,
    sinf_root_mod = 0
  },
  {
    name = "Minor",
    number = 3,
    scale = musicutil.generate_scale(0, "minor", 1),
    romans = {"i", "ii°", "♭III", "iv", "v", "♭VI", "♭VII"},
    sinf_degrees = {0, 2, 3, 5, 7, 8, 10},
    sinf_mode = 4,
    sinf_root_mod = 0
  },
  {
    name = "Harmonic Minor",
    number = 4,
    scale = musicutil.generate_scale(0, "harmonic minor", 1),
    romans = {"i", "ii°", "♭III+", "iv", "V", "♭VI", "vii°"},
    sinf_degrees = {0, 2, 3, 5, 7, 8, 11},
    sinf_mode = 6,
    sinf_root_mod = 0
  },
  {
    name = "Melodic Minor",
    number = 5,
    scale = musicutil.generate_scale(0, "melodic minor", 1),
    romans = {"i", "ii", "♭III+", "IV", "V", "vi°", "vii°"},
    sinf_degrees = {0, 2, 3, 5, 7, 9, 11},
    sinf_mode = 5,
    sinf_root_mod = 0
  },
  {
    name = "Dorian",
    number = 6,
    scale = musicutil.generate_scale(0, "dorian", 1),
    romans = {"i", "ii", "♭III", "IV", "v", "vi°", "♭VII"},
    sinf_degrees = {2, 4, 5, 7, 9, 11, 0},
    sinf_mode = 3,
    sinf_root_mod = 10
  },
  {
    name = "Phrygian",
    number = 7,
    scale = musicutil.generate_scale(0, "phrygian", 1),
    romans = {"i", "♭II", "♭III", "iv", "v°", "♭VI", "♭VII"},
    sinf_degrees = {4, 5, 7, 9, 11, 0, 2},
    sinf_mode = 3,
    sinf_root_mod = 8
  },
  {
    name = "Lydian",
    number = 8,
    scale = musicutil.generate_scale(0, "lydian", 1),
    romans = {"I", "II", "iii", "#IV°", "V", "vi", "vii"},
    sinf_degrees = {5, 7, 9, 11, 0, 2, 4},
    sinf_mode = 3,
    sinf_root_mod = 7
  },
  {
    name = "Mixolydian",
    number = 9,
    scale = musicutil.generate_scale(0, "mixolydian", 1),
    romans = {"I", "ii", "iii°", "IV", "v", "vi", "♭VII"},
    sinf_degrees = {7, 9, 11, 0, 2, 4, 5},
    sinf_mode = 3,
    sinf_root_mod = 5
  },
  {
    name = "Locrian",
    number = 10,
    scale = musicutil.generate_scale(0, "locrian", 1),
    romans = {"i°", "♭II", "♭iii", "iv", "♭V", "♭VI", "♭VII"},
    sinf_degrees = {11, 0, 2, 4, 5, 7, 9},
    sinf_mode = 3,
    sinf_root_mod = 1
  }
}
```

Send the program change message to channel 1 to set the root_note, 2 to set the degree, 3: to set the mode, 4 to set the transposition. The program change value will set the parameter to the desired setting.
