from time import sleep
from agent.agent import Agent
import sys
import os
import signal

agents = []
child_pids = []


def run_agent(ip, port, team, agent_id, performance):
  try:
    agent = Agent(ip, port, team, agent_id, performance)
    agents.append(agent)
    agent.start()
  except Exception as e:
    print(f"Agent {agent_id} failed: {e}")
    sys.exit(1)


def signal_handler(sig, frame):
  print("Received termination signal. Stopping all agents...")
  for agent in agents:
    try:
      agent.stop()
    except Exception as e:
      print(f"Error stopping agent: {e}")

  for pid in child_pids:
    try:
      os.kill(pid, signal.SIGTERM)
      print(f"Sent SIGTERM to process {pid}")
    except ProcessLookupError:
      pass
  sys.exit(0)


def fork_agent(ip, port, team, i, performance):
  pid = os.fork()
  if pid == 0:
    run_agent(ip, port, team, i, performance)
    sys.exit(0)
  else:
    child_pids.append(pid)
    print(f"Started agent {i} with PID {pid}")
    sleep(0.01)


def wait_childs():
  try:
    for pid in child_pids:
      os.waitpid(pid, 0)
  except KeyboardInterrupt:
    pass
