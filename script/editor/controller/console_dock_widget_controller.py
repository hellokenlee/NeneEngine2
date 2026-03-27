# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import logging
import html
import re

from PySide6.QtWidgets import QTextBrowser

from script.editor.common.log import global_logger, global_log_formatter
from script.editor.controller.dock_widget_controller import DockWidgetController

from nene import EventSubscriber, Logger, LogMessageEvent

_LOG_PREFIX_PATTERN = re.compile(
	r"^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\s+"
	r"(?P<level>debug|info|warn|warning|error|critical)\s+",
	re.IGNORECASE
)

_LEVEL_TOKEN_PATTERN = re.compile(
	r"^(?P<level>debug|info|warn|warning|error|critical)\s+",
	re.IGNORECASE
)

_LEVEL_COLORS = {
	logging.DEBUG: "#90A4AE",
	logging.INFO: None,
	logging.WARNING: "#FFB74D",
	logging.ERROR: "#EF5350",
	logging.CRITICAL: "#D32F2F",
}


def _normalize_level_from_text(level_str: str):
	level_name = level_str.lower()
	if level_name == "warn":
		level_name = "warning"
	return getattr(logging, level_name.upper(), None)


def _strip_prefixes(message: str):
	match = _LOG_PREFIX_PATTERN.match(message)
	if match:
		levelno = _normalize_level_from_text(match.group("level"))
		return message[match.end():], levelno

	match = _LEVEL_TOKEN_PATTERN.match(message)
	if match:
		levelno = _normalize_level_from_text(match.group("level"))
		return message[match.end():], levelno

	return message, None


def _append_colored_log(text_browser: QTextBrowser, message: str, levelno=None):
	content, parsed_levelno = _strip_prefixes(message.strip())
	actual_level = levelno if levelno is not None else parsed_levelno
	if actual_level is None:
		actual_level = logging.INFO
	color = _LEVEL_COLORS.get(actual_level, _LEVEL_COLORS[logging.INFO])
	if color is None:
		text_browser.append(content)
	else:
		text_browser.append(
			f"<span style='color: {color};'>{html.escape(content)}</span>"
		)


class EngineLogHandler(EventSubscriber):
	def __init__(self, text_browser: QTextBrowser):
		super().__init__()
		self.text_browser = text_browser
		Logger.publisher().add_subscriber(self)
		pass

	def on_notify(self, e):
		if isinstance(e, LogMessageEvent):
			_append_colored_log(self.text_browser, e.m_message)
		pass


class PythonLogHandler(logging.Handler):
	def __init__(self, text_browser: QTextBrowser):
		super().__init__()
		self.text_browser = text_browser
		#
		self.setFormatter(global_log_formatter)
		# 替换缓冲的日志处理器
		if global_logger.hasHandlers():
			buff_handler = global_logger.handlers[0]
			global_logger.removeHandler(buff_handler)
			buff_handler.buffer.seek(0)
			for line in buff_handler.buffer:
				_append_colored_log(self.text_browser, line[:-1])
		#
		global_logger.addHandler(self)
		pass

	def __del__(self):
		global_logger.removeHandler(self)
		pass

	def emit(self, record):
		msg = self.format(record)
		_append_colored_log(self.text_browser, msg, record.levelno)
		pass


class ConsoleDockWidgetController(DockWidgetController):

	UI_FILE = "console_dock_widget.ui"

	def __init__(self):
		super(ConsoleDockWidgetController, self).__init__()
		self.log_text_browser = self.ui.findChild(QTextBrowser, "logTextBrowser")
		#
		self.engine_log_handler = EngineLogHandler(self.log_text_browser)
		self.python_log_handler = PythonLogHandler(self.log_text_browser)
		pass
