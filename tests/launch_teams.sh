#!/bin/bash
# Script to launch 4 zappy_ai clients in 4 terminals for teams caca and caca2

# Path to the AI client (relative to this script's location)
AI_PATH="/home/toujourspasmoi/temp_zappy/DFMY-Zappy/zappy_ai"
PORT=4242
TEAM1="caca"
TEAM2="caca2"

# use gnome-terminal for gnome environnement
# Launch 2 clients for each team in separate QTerminal windows
qterminal -e bash -c "$AI_PATH -p $PORT -n $TEAM1; exec bash" &
qterminal -e bash -c "$AI_PATH -p $PORT -n $TEAM1; exec bash" &
qterminal -e bash -c "$AI_PATH -p $PORT -n $TEAM2; exec bash" &
qterminal -e bash -c "$AI_PATH -p $PORT -n $TEAM2; exec bash" &

wait
