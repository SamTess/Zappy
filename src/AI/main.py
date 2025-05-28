#!/usr/bin/env python3
import sys
from ParsingArgs.paringArgsClass import ParsingArgs

if __name__ == "__main__":
    args = ParsingArgs()
    print(f"Port: {args.getPort()}")
    print(f"Name: {args.getName()}")
    print(f"Machine: {args.getMachine()}")
    print("IA started successfully!")
    exit(0)

