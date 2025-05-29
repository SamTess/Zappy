from enum import Enum, IntEnum
import datetime

class LogLevel(IntEnum):
    DEBUG = 10
    INFO = 20
    WARNING = 30
    ERROR = 40
    CRITICAL = 50

class Logger:
    def __init__(self, fileName: str = "AI.log"):
        self.file = fileName
        self.file_handle = open(fileName, "a")

    def log(self, message: str, level: LogLevel):
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        level_name = level.name
        self.file_handle.write(f"[{timestamp}] {level_name}: {message}\n")
        self.file_handle.flush()

    def debug(self, message: str):
        self.log(message, LogLevel.DEBUG)

    def info(self, message: str):
        self.log(message, LogLevel.INFO)

    def warning(self, message: str):
        self.log(message, LogLevel.WARNING)

    def error(self, message: str):
        self.log(message, LogLevel.ERROR)

    def critical(self, message: str):
        self.log(message, LogLevel.CRITICAL)

