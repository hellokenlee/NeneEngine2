# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import io
import logging
import builtins
import sys

global_logger = logging.getLogger()

INFO = logging.INFO
DEBUG = logging.DEBUG
ERROR = logging.ERROR
WARNING = logging.WARNING

LOG_FORMAT = '%(asctime)s %(message)s'
LOG_DATE_FORMAT = '%Y-%m-%d %H:%M:%S'

global_log_formatter = logging.Formatter(LOG_FORMAT, datefmt=LOG_DATE_FORMAT)
_original_print = builtins.print
_print_hijacked = False
_print_cat = "print"
_print_level = INFO


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
		WARNING: "warn",
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


def _print_proxy(*values, sep=' ', end='\n', file=None, flush=False):
	target_stream = file if file is not None else sys.stdout
	if target_stream not in (sys.stdout, sys.stderr):
		_original_print(*values, sep=sep, end=end, file=file, flush=flush)
		return
	message = sep.join(str(v) for v in values)
	if end and end != '\n':
		message += end
	level = ERROR if target_stream is sys.stderr else _print_level
	try:
		log(_print_cat, level, message)
	except Exception:
		_original_print(*values, sep=sep, end=end, file=file, flush=flush)
		return
	if flush:
		for handler in global_logger.handlers:
			handler.flush()
	pass


def hijack_print(cat: object = "print", level: int = INFO):
	global _print_hijacked
	global _print_cat
	global _print_level
	_print_cat = cat
	_print_level = level
	if _print_hijacked:
		return
	builtins.print = _print_proxy
	_print_hijacked = True
	pass


def restore_print():
	global _print_hijacked
	if not _print_hijacked:
		return
	builtins.print = _original_print
	_print_hijacked = False
	pass
