#!/usr/bin/env python3
import sys
import threading
from time import sleep
from agent.agent import Agent
from parser.paringArgsClass import parseArgs

def run_agent(ip, port, team, agent_id):
    try:
        ai = Agent(ip, port, team, agent_id)
        ai.run()
    except Exception as e:
        print(f"Agent {agent_id}: Failed to start: {e}")

if __name__ == "__main__":
    args = parseArgs()
    num_agents = 1
    port = args.getPort()
    ip = args.getMachine()
    team = args.getName()

    print(f"Starting {num_agents} agents for team {team} on {ip}:{port}")

    threads = []

    for i in range(num_agents):
        thread = threading.Thread(target=run_agent, args=(ip, port, team, i))
        thread.daemon = True
        threads.append(thread)
        thread.start()
        print(f"Started agent {i}")
        sleep(0.1)
    print("IA started successfully!")

    try:
        for thread in threads:
            thread.join()
    except KeyboardInterrupt:
        print("\nShutting down all agents...")
        sys.exit(0)

    exit(0)
