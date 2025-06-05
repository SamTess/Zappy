#!/usr/bin/env python3
import sys
import os
import signal
from time import sleep
from agent.agent import Agent
from parser.paringArgsClass import parseArgs


def run_agent(ip, port, team, agent_id):
  try:
    ai = Agent(ip, port, team, agent_id)
    ai.start()
  except Exception as e:
    print(f"Agent {agent_id} failed: {e}")
    sys.exit(1)

def signal_handler(sig, frame):
  for pid in child_pids:
    try:
      os.kill(pid, signal.SIGTERM)
    except ProcessLookupError:
      pass
  sys.exit(0)

if __name__ == "__main__":

  for i in range(17):
    get_nb_rows_to_item(i)
  # print("Number of rows to reach item:", nb_rows)
  if 1:
    sys.exit(0)

  args = parseArgs()
  num_agents = 1
  port = args.getPort()
  ip = args.getMachine()
  team = args.getName()

  print(f"Starting {num_agents} agents for team {team} on {ip}:{port}")

  child_pids = []

  signal.signal(signal.SIGINT, signal_handler)
  signal.signal(signal.SIGTERM, signal_handler)

  for i in range(num_agents):
    pid = os.fork()

    if pid == 0:
      run_agent(ip, port, team, i)
      sys.exit(0)
    else:
      child_pids.append(pid)
      print(f"Started agent {i} with PID {pid}")
      sleep(0.1)

  print("IA started successfully!")

  try:
    for pid in child_pids:
      os.waitpid(pid, 0)
  except KeyboardInterrupt:
    pass
  print("All agents completed")
