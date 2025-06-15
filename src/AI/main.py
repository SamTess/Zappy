#!/usr/bin/env python3
import sys
import os
import signal
from time import sleep
from agent.agent import Agent
from parser.paringArgsClass import parseArgs

agents = []

def run_agent(ip, port, team, agent_id, performance):
  try:
    ai = Agent(ip, port, team, agent_id, performance)
    agents.append(ai)
    ai.start()
  except Exception as e:
    print(f"Agent {agent_id} failed: {e}")
    sys.exit(1)

def signal_handler(sig, frame):
  print("Received termination signal. Stopping all agents...")
  for ai in agents:
    try:
      ai.stop()
    except Exception as e:
      print(f"Error stopping agent: {e}")

  for pid in child_pids:
    try:
      os.kill(pid, signal.SIGTERM)
      print(f"Sent SIGTERM to process {pid}")
    except ProcessLookupError:
      pass
  sys.exit(0)

if __name__ == "__main__":
  args = parseArgs()
  num_agents = 10
  port = args.getPort()
  ip = args.getMachine()
  team = args.getName()
  performance = args.getPerformance()

  print(f"Starting {num_agents} agents for team {team} on {ip}:{port}")
  if performance:
    print("Performance mode enabled")

  child_pids = []

  signal.signal(signal.SIGINT, signal_handler)
  signal.signal(signal.SIGTERM, signal_handler)

  for i in range(num_agents):
    pid = os.fork()

    if pid == 0:
      run_agent(ip, port, team, i, performance)
      sys.exit(0)
    else:
      child_pids.append(pid)
      print(f"Started agent {i} with PID {pid}")
      sleep(0.01)

  print("IA started successfully!")

  try:
    for pid in child_pids:
      os.waitpid(pid, 0)
  except KeyboardInterrupt:
    pass
  print("All agents completed")
