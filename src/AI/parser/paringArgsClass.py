
import sys

class ParsingArgsError(Exception):
    def __init__(self, message="Erreur lors du parsing des arguments"):
        self.message = message
        print(f"ParsingArgsError: {message}", file=sys.stderr)
        super().__init__(self.message)

class parseArgs:
    def __init__(self):
        self.args = sys.argv[1:]
        self.port = 0
        self.name = ""
        self.machine = "localhost"
        try:
            if (len(self.args) == 0):
                raise ParsingArgsError("No arguments provided. Use -p <port> -n <name> -h <machine> to specify the required parameters.")
            self.parsed_args = self.parseArgs()
        except ParsingArgsError:
            sys.exit(84)


    def hasFlagAndValue(self, flag):
        for i, arg in enumerate(self.args):
            if arg == flag:
                if i + 1 < len(self.args):
                    if not self.args[i + 1].startswith('-'):
                        return True
                    else:
                        raise ParsingArgsError(f"Flag {flag} requires a value, but got another flag: {self.args[i + 1]}")
                else:
                    raise ParsingArgsError(f"Flag {flag} requires a value.")
        return False

    def getFlagValue(self, flag):
        for i, arg in enumerate(self.args):
            if arg == flag and i + 1 < len(self.args):
                return self.args[i + 1]
        return None

    def searchUnknownFlag(self, flag):
        i = 0
        while (i < len(self.args)):
            if self.args[i] not in flag:
                raise ParsingArgsError(f"Unknown argument: {self.args[i]}")
            i += 2

    def parseArgs(self):
        # add flags if wanted
        self.searchUnknownFlag(["-p", "-n", "-h"])
        # maybe to a for loop to check if the flags are in the args
        if (self.hasFlagAndValue("-p")):
            self.setPort(self.getFlagValue("-p"))
        else:
            raise ParsingArgsError("No port specified. Use -p <port> to specify a port.")

        if (self.hasFlagAndValue("-n")):
            self.setName(self.getFlagValue("-n"))
        else:
            raise ParsingArgsError("No name specified. Use -n <name> to specify a name.")

        if (self.hasFlagAndValue("-h")):
            self.setMachine(self.getFlagValue("-h"))
        else:
            print("No machine specified, using default machine 'localhost'")

    def getPort(self):
        return self.port

    def setPort(self, port):
        try:
            port_int = int(port)
            if (port_int < 1 or port_int > 65535):
                raise ParsingArgsError("Invalid port number. Port must be between 1 and 65535.")
            self.port = port_int
        except ValueError:
            raise ParsingArgsError(f"Invalid port format: {port}. Port must be a number.")

    def getName(self):
        return self.name

    def setName(self, name):
        if not isinstance(name, str):
            raise ParsingArgsError("Name must be a string.")
        self.name = name

    def getMachine(self):
        return self.machine

    def setMachine(self, machine):
        if not isinstance(machine, str):
            raise ParsingArgsError("Machine must be a string.")
        self.machine = machine