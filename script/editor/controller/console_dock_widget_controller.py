# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import logging

from PySide6.QtWidgets import QTextBrowser

from script.editor.common.log import global_logger, global_log_formatter
from script.editor.controller.dock_widget_controller import DockWidgetController

from nene import EventSubscriber, Logger, LogMessageEvent


class EngineLogHandler(EventSubscriber):
	def __init__(self, text_browser: QTextBrowser):
		super().__init__()
		self.text_browser = text_browser
		Logger.publisher().add_subscriber(self)
		pass

	def on_notify(self, e):
		if isinstance(e, LogMessageEvent):
			self.text_browser.append(e.m_message)
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
				self.text_browser.append(line[:-1])
		#
		global_logger.addHandler(self)
		pass

	def __del__(self):
		global_logger.removeHandler(self)
		pass

	def emit(self, record):
		msg = self.format(record)
		self.text_browser.append(msg)
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

