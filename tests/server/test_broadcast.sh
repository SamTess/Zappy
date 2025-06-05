#!/bin/bash

# Zappy Broadcast Test Script
# Connects 5 players to the server and tests broadcast functionality

SERVER_IP="127.0.0.1"
SERVER_PORT="4242"
TEAM_NAME="test_team"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}Starting Zappy Broadcast Test${NC}"
echo -e "${YELLOW}Connecting 5 players to server at ${SERVER_IP}:${SERVER_PORT}${NC}"

# Function to connect a player and keep it alive
connect_player() {
    local player_id=$1
    local send_broadcast=$2
    
    echo -e "${GREEN}Connecting Player $player_id...${NC}"
    
    if [ "$send_broadcast" = "true" ]; then
        # Player 1 sends a broadcast after connecting
        (
            sleep 2
            echo "Broadcast Hello from Player $player_id! This is a test message."
            sleep 30  # Keep connection alive
        ) | telnet $SERVER_IP $SERVER_PORT > /tmp/player_${player_id}_output.log 2>&1 &
        echo -e "${YELLOW}Player $player_id will send broadcast message${NC}"
        echo -e "${BLUE}Player $player_id output will be logged to /tmp/player_${player_id}_output.log${NC}"
    else
        # Other players just connect and listen
        (
            sleep 30  # Keep connection alive
        ) | telnet $SERVER_IP $SERVER_PORT > /tmp/player_${player_id}_output.log 2>&1 &
        echo -e "${GREEN}Player $player_id connected and listening${NC}"
        echo -e "${BLUE}Player $player_id output will be logged to /tmp/player_${player_id}_output.log${NC}"
    fi
    
    # Store the PID for cleanup
    PIDS[$player_id]=$!
}

# Array to store process IDs
declare -a PIDS

# Connect 5 players
for i in {1..5}; do
    if [ $i -eq 1 ]; then
        connect_player $i true
    else
        connect_player $i false
    fi
    sleep 0.5  # Small delay between connections
done

echo -e "${BLUE}All players connected. Player 1 will send broadcast in 2 seconds.${NC}"
echo -e "${YELLOW}Press Ctrl+C to stop all connections${NC}"
echo -e "${BLUE}Logs will be available in /tmp/player_X_output.log files${NC}"

# Function to show logs
show_logs() {
    echo -e "\n${BLUE}=== Player Logs ===${NC}"
    for i in {1..5}; do
        if [ -f "/tmp/player_${i}_output.log" ]; then
            echo -e "${GREEN}Player $i output:${NC}"
            cat "/tmp/player_${i}_output.log"
            echo -e "${YELLOW}--- End Player $i ---${NC}\n"
        fi
    done
}

# Function to cleanup on exit
cleanup() {
    echo -e "\n${RED}Cleaning up connections...${NC}"
    for pid in "${PIDS[@]}"; do
        if kill -0 $pid 2>/dev/null; then
            kill $pid 2>/dev/null
        fi
    done
    echo -e "${GREEN}All connections closed.${NC}"
    exit 0
}

# Set trap for cleanup on script exit
trap cleanup SIGINT SIGTERM

# Wait for all background processes
wait
