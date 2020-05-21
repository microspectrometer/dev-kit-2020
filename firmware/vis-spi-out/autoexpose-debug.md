# Setup

Run algorithm one iteration at a time using
`kit-gui.py`:

- SetAutoExposeConfig(`max_tries` = 1)
- hitting the spacebar calls AutoExposure()

Screen displays `input_peak` and `exposure`.

Record `input_peak`, `exposure`, and behavior:
where the final peak, after applying exposure, in
terms of `max_dark` and `target`.

# Data Collection
For each recorded line below,

- algorithm starts with `input_peak`
- algorithm calculates exposure from `input_peak`
- `kit-gui.py` immediately applies the new
  exposure by looping CaptureFrame()
- observe the behavior of applying the new
  exposure -- this is the input peak for the next
  iteration

# Condition: no light input

input-peak, cycles, result, notes
1417,500, below max dark, normal
1361,5000,below max dark, normal
2036,5000,below min peak, normal
10312,4,below max dark, expected cycles=65535
1241,40,below max dark, normal
1392,400,below max dark, normal
1437,4000, below max dark, normal
2005, 40000, below min peak, normal
8378, 4, below max dark, expected cycles=65535

# Condition: white-led

# Condition: white-led
