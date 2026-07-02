# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import logging
import html
import re
import traceback

from PySide6 import QtCore
from PySide6.QtGui import QKeyEvent
from PySide6.QtCore import QObject, QEvent, Qt
from PySide6.QtWidgets import QTextBrowser, QLineEdit

from script.editor.common.log import global_logger, global_log_formatter
from script.editor.widget.base_dock_widget import BaseDockWidget

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


class _EngineLogHandler(EventSubscriber):
	def __init__(self, text_browser: QTextBrowser):
		super().__init__()
		self.text_browser = text_browser
		Logger.publisher().add_subscriber(self)
		pass

	def on_notify(self, e):
		if isinstance(e, LogMessageEvent):
			_append_colored_log(self.text_browser, e.m_message)
		pass


class _PythonLogHandler(logging.Handler):
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


class _CommandLineHistoryFilter(QObject):
	def __init__(self, widget, parent=None):
		super().__init__(parent)
		self._widget = widget

	def eventFilter(self, watched, event):
		if event.type() == QEvent.Type.KeyPress:
			assert (isinstance(event, QKeyEvent))
			if event.key() == Qt.Key.Key_Up:
				self._widget.restore_prev_command()
				return True
			if event.key() == Qt.Key.Key_Down:
				self._widget.restore_next_command()
				return True
		return False


class ConsoleDockWidget(BaseDockWidget):

	UI_FILE = "console_dock_widget.ui"

	def __init__(self, parent):
		super().__init__(parent)
		self._command_line_edit: QLineEdit = QLineEdit()
		self.log_text_browser: QTextBrowser = QTextBrowser()
		self._python_globals = {}
		self._python_locals = {}
		self._command_history = []
		self._history_cursor = 0
		self._history_draft = ""
		self._command_history_filter = None
		self._engine_log_handler = None
		self._python_log_handler = None
		pass

	def setup(self):
		super().setup()
		self._command_line_edit = self.find_child(QLineEdit, "commandLineEdit")
		self.log_text_browser = self.find_child(QTextBrowser, "logTextBrowser")
		self._python_globals = {"__builtins__": __builtins__}
		self._python_locals = {"widget": self}
		#
		self._engine_log_handler = _EngineLogHandler(self.log_text_browser)
		self._python_log_handler = _PythonLogHandler(self.log_text_browser)
		if self._command_line_edit:
			self._command_history_filter = _CommandLineHistoryFilter(self, self._command_line_edit)
			self._command_line_edit.installEventFilter(self._command_history_filter)
			self._command_line_edit.returnPressed.connect(self._on_command_line_return_pressed)
			self._command_line_edit.textEdited.connect(self._on_command_text_edited)
		pass

	def _on_command_text_edited(self, _):
		self._history_cursor = len(self._command_history)
		if self._command_line_edit is not None:
			self._history_draft = self._command_line_edit.text()
		pass

	def restore_prev_command(self):
		if self._command_line_edit is None or not self._command_history:
			return
		if self._history_cursor == len(self._command_history):
			self._history_draft = self._command_line_edit.text()
		if self._history_cursor > 0:
			self._history_cursor -= 1
		command = self._command_history[self._history_cursor]
		self._command_line_edit.setText(command)
		self._command_line_edit.setCursorPosition(len(command))
		pass

	def restore_next_command(self):
		if self._command_line_edit is None or not self._command_history:
			return
		last_index = len(self._command_history) - 1
		if self._history_cursor < last_index:
			self._history_cursor += 1
			command = self._command_history[self._history_cursor]
		else:
			self._history_cursor = len(self._command_history)
			command = self._history_draft
		self._command_line_edit.setText(command)
		self._command_line_edit.setCursorPosition(len(command))
		pass

	def _on_command_line_return_pressed(self):
		if self._command_line_edit is None or self.log_text_browser is None:
			return
		command = self._command_line_edit.text().strip()
		if not command:
			return

		self._command_history.append(command)
		self._history_cursor = len(self._command_history)
		self._history_draft = ""
		self._command_line_edit.clear()
		_append_colored_log(self.log_text_browser, ">>> %s" % command, logging.DEBUG)

		try:
			try:
				code = compile(command, "<console>", "eval")
				result = eval(code, self._python_globals, self._python_locals)
				if result is not None:
					_append_colored_log(self.log_text_browser, repr(result), logging.INFO)
			except SyntaxError:
				try:
					code = compile(command, "<console>", "exec")
					exec(code, self._python_globals, self._python_locals)
				except SyntaxError as syntax_error:
					raise syntax_error from None
		except Exception:
			for line in traceback.format_exc().rstrip().splitlines():
				_append_colored_log(self.log_text_browser, line, logging.ERROR)
		pass
