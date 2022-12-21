import logging

level = logging.DEBUG
TRACE = logging.DEBUG - 5
existing_loggers = {}


def add_logging_level(level_name, level_num, method_name=None):
    if not method_name:
        method_name = level_name.lower()

    if hasattr(logging, level_name):
        print("Logging level '{}' already defined".format(str(level_name)))
        return
    if hasattr(logging, method_name):
        raise AttributeError(
            '{} already defined in logging module'.format(method_name))
    if hasattr(logging.getLoggerClass(), method_name):
        raise AttributeError(
            '{} already defined in logger class'.format(method_name))

    def log_for_level(self, message, *args, **kwargs):
        if self.isEnabledFor(level_num):
            self._log(level_num, message, args, **kwargs)

    def log_to_root(message, *args, **kwargs):
        logging.log(level_num, message, *args, **kwargs)

    logging.addLevelName(level_num, level_name)
    setattr(logging, level_name, level_num)
    setattr(logging.getLoggerClass(), method_name, log_for_level)
    setattr(logging, method_name, log_to_root)


add_logging_level('TRACE', TRACE)


def set_level(level_raw):
    global level
    level = getattr(logging, level_raw.upper())


class Formatter(logging.Formatter):

    @staticmethod
    def get_format(color):
        return color + "%(asctime)s - %(name)s - " + Colors.RESET + "%(message)s"

    def format(record):
        FORMATS = {
            logging.TRACE: Formatter.get_format(Colors.GRAY),
            logging.DEBUG: Formatter.get_format(Colors.RESET),
            logging.INFO: Formatter.get_format(Colors.GREEN),
            logging.WARNING: Formatter.get_format(Colors.YELLOW),
            logging.ERROR: Formatter.get_format(Colors.RED),
            logging.CRITICAL: Formatter.get_format(Colors.BOLD_RED)
        }
        log_fmt = FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)


def factory(name):
    global level
    if name in existing_loggers:
        return existing_loggers[name]
    logger = logging.getLogger(name)
    logger_handler = logging.StreamHandler()
    logger.addHandler(logger_handler)
    logger_handler.setFormatter(Formatter)
    logger.setLevel(level)
    existing_loggers[name] = logger
    return logger


class Colors:
    """ ANSI color codes 
    https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
    """
    RED = "\x1b[38;5;196m"
    GREEN = "\x1b[38;5;70m"
    GRAY = "\x1b[38;5;240m"
    BOLD_RED = "\x1b[1;38;5;196m"
    YELLOW = "\x1b[38;5;220m"
    RESET = "\x1b[0m"
