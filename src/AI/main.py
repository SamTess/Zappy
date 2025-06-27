#!/usr/bin/env python3

import signal
import utils.mutliprocessing as mutliprocessing
from agent.agent import Agent
from parser.paringArgsClass import parseArgs

if __name__ == "__main__":
  args = parseArgs()
  port = args.getPort()
  ip = args.getMachine()
  team = args.getName()
  performance = args.getPerformance()

  print(f"Starting the first agent of the team {team} on {ip}:{port}")

  if performance:
    print("Performance mode enabled")

  signal.signal(signal.SIGINT, mutliprocessing.signal_handler)
  signal.signal(signal.SIGTERM, mutliprocessing.signal_handler)

  mutliprocessing.run_agent(ip, port, team, performance)

  print("IA started successfully!")

  mutliprocessing.wait_childs()

  print("All agents completed")
