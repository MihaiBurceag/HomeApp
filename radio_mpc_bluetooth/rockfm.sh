#!/bin/bash
if mpc status | grep playing >/dev/nul      # If mpd is playing
then
mpc clear
mpc add http://80.86.106.143:9128/rockfm.aacp
mpc play
else
mpc clear
mpc add http://80.86.106.143:9128/rockfm.aacp
fi



