from time import sleep
from agent.agent import Agent
import sys
import os
import signal
import threading
import atexit

agents = []
child_pids = []
shutdown_flag = threading.Event()


def run_agent(ip, port, team, agent_id, performance):
  try:
    agent = Agent(ip, port, team, agent_id, performance)
    agents.append(agent)
    agent.start()
  except Exception as e:
    print(f"Agent {agent_id} failed: {e}")
    os._exit(1)


def signal_handler(sig, frame):
  if shutdown_flag.is_set():
    return

  shutdown_flag.set()
  print("\nReceived termination signal. Stopping all agents...")

  for agent in agents:
    try:
      if hasattr(agent, 'running') and agent.running:
        agent.stop()
    except Exception as e:
      print(f"Error stopping agent: {e}")

  for pid in child_pids:
    try:
      os.kill(pid, signal.SIGTERM)
      print(f"Sent SIGTERM to process {pid}")
    except ProcessLookupError:
      print(f"Process {pid} already terminated")
    except Exception as e:
      print(f"Error terminating process {pid}: {e}")

  sleep(0.5)

  for pid in child_pids:
    try:
      os.kill(pid, signal.SIGKILL)
    except ProcessLookupError:
      pass
    except Exception:
      pass

  print("All agents stopped.")
  os._exit(0)


def cleanup_handler():
  """Cleanup function called on normal exit"""
  if not shutdown_flag.is_set():
    signal_handler(signal.SIGTERM, None)


def fork_agent(ip, port, team, i, performance):
  pid = os.fork()
  if pid == 0:
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGTERM, signal.SIG_DFL)

    try:
      run_agent(ip, port, team, i, performance)
    except Exception as e:
      print(f"Agent {i} crashed: {e}")
    finally:
      os._exit(0)
  else:
    child_pids.append(pid)
    print(f"Started agent {i} with PID {pid}")
    sleep(0.01)


def wait_childs():
  signal.signal(signal.SIGINT, signal_handler)
  signal.signal(signal.SIGTERM, signal_handler)
  atexit.register(cleanup_handler)

  try:
    while child_pids and not shutdown_flag.is_set():
      for pid in child_pids[:]:
        try:
          result = os.waitpid(pid, os.WNOHANG)
          if result[0] != 0:
            child_pids.remove(pid)
            print(f"Agent with PID {pid} terminated")
        except OSError:
          if pid in child_pids:
            child_pids.remove(pid)

      if child_pids:
        sleep(0.1)

  except KeyboardInterrupt:
    print("\nKeyboard interrupt received")
    signal_handler(signal.SIGINT, None)
  except Exception as e:
    print(f"Error in wait_childs: {e}")
    signal_handler(signal.SIGTERM, None)
