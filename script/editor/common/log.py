# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import io
import logging

global_logger = logging.getLogger()

INFO = logging.INFO
DEBUG = logging.DEBUG
ERROR = logging.ERROR
WARNING = logging.WARNING

LOG_FORMAT = '%(asctime)s %(message)s'
LOG_DATE_FORMAT = '%Y-%m-%d %H:%M:%S'

global_log_formatter = logging.Formatter(LOG_FORMAT, datefmt=LOG_DATE_FORMAT)


class BufferHandler(logging.Handler):
	def __init__(self):
		super().__init__()
		self.buffer = io.StringIO()
		self.setFormatter(global_log_formatter)
		pass

	def emit(self, record):
		self.buffer.write(self.format(record) + "\n")
		pass


def log(cat: object, level: int, message: str):
	level_strs = {
		INFO: "info",
		DEBUG: "debug",
		ERROR: "error",
		WARNING: "warning",
	}

	if not global_logger.hasHandlers():
		global_logger.setLevel(logging.DEBUG)
		global_logger.addHandler(BufferHandler())

	if cat is None:
		global_logger.log(level, message)
	elif isinstance(cat, str):
		global_logger.log(level, "%s [%s] %s" % (level_strs[level], cat, message))
	elif isinstance(cat, type):
		global_logger.log(level, "%s [%s] %s" % (level_strs[level], cat.__name__, message))
	else:
		global_logger.log(level, "%s [%s] %s" % (level_strs[level], cat.__class__.__name__, message))
	pass
