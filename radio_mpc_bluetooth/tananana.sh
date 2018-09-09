#!/bin/bash
if mpc status | grep playing >/dev/nul      # If mpd is playing
then
mpc clear
mpc add http://live.tananana.ro:8010/stream-48.aac
mpc play
else
mpc clear
mpc add http://live.tananana.ro:8010/stream-48.aac
fi
