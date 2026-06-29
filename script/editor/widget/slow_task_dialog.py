# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import time

from script.editor.common import util
from PySide6.QtCore import Qt, QTimer
from PySide6.QtWidgets import QDialog, QLabel, QProgressBar, QVBoxLayout, QHBoxLayout, QApplication

from nene import ScopedSlowTask

# noinspection DuplicatedCode
class SlowTaskDialog(QDialog):
	"""阻塞式的耗时任务进度对话框。

	布局大致分为三行：
		- 第一行：左对齐的任务标题 + 右对齐的百分比进度。
		- 第二行：可以不断递进的进度条。
		- 第三行：小字显示当前正在进行的子任务。
	"""

	def __init__(self, title: str = "SlowTask"):
		# 自动以主窗口作为父对象，使对话框居中于主窗口之上并纳入其对象树。
		super().__init__(util.find_qt_main_window())
		self._total = 100
		self._value = 0
		# exec() 期间用于不断驱动 on_update 的定时器。
		self._update_timer: QTimer | None = None
		#
		self.setWindowTitle(title)
		self.setModal(True)
		self.setFixedSize(360, 110)
		# 无边框：隐藏系统标题栏，仅保留内容区域。
		self.setWindowFlag(Qt.WindowType.FramelessWindowHint, True)
		# 阻塞期间禁止关闭按钮与右上角退出。
		self.setWindowFlag(Qt.WindowType.WindowCloseButtonHint, False)
		self.setWindowFlag(Qt.WindowType.WindowContextHelpButtonHint, False)
		# 无边框时补一圈边框，避免与背景融为一体。
		self.setStyleSheet("SlowTaskDialog{border:1px solid #555555;}")
		#
		layout = QVBoxLayout(self)
		layout.setContentsMargins(12, 12, 12, 12)
		layout.setSpacing(6)
		# 第一行：左对齐标题 + 右对齐百分比。
		header_layout = QHBoxLayout()
		header_layout.setContentsMargins(0, 0, 0, 0)
		header_layout.setSpacing(4)
		self._title_label = QLabel(title, self)
		self._title_label.setAlignment(Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)
		header_layout.addWidget(self._title_label, 1)
		self._percent_label = QLabel("0%", self)
		self._percent_label.setAlignment(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter)
		header_layout.addWidget(self._percent_label, 0)
		layout.addLayout(header_layout)
		# 第二行：进度条。
		self._progress_bar = QProgressBar(self)
		self._progress_bar.setRange(0, self._total)
		self._progress_bar.setValue(0)
		self._progress_bar.setTextVisible(False)
		layout.addWidget(self._progress_bar)
		# 第三行：小字的当前子任务描述。
		self._detail_label = QLabel("", self)
		self._detail_label.setAlignment(Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)
		self._detail_label.setStyleSheet("QLabel{color:#999999;font-size:11px;}")
		layout.addWidget(self._detail_label)
		pass

	def on_update(self) -> bool:
		"""更新函数：在 exec() 阻塞期间会被反复调用。

		在这里推进进度（set_progress / advance / set_detail），
		并在任务完成时返回 True，对话框便会自动退出阻塞。
		"""
		if not ScopedSlowTask.is_progress_queue_empty():
			msg, percentage = ScopedSlowTask.pop_progress_percentage()
			self._set_progress(percentage * 100, "%s..." % msg)
			if percentage >= 1.0:
				return True
		return False

	def exec(self) -> int:
		"""以阻塞方式显示对话框，并在内部事件循环里反复调用 on_update。

		on_update 返回 True 时自动 accept()，exec() 随即返回。
		"""
		timer = QTimer(self)
		timer.setInterval(0)
		timer.timeout.connect(self._on_tick)
		timer.start()
		self._update_timer = timer
		try:
			return super().exec()
		finally:
			timer.stop()
			self._update_timer = None

	def _on_tick(self) -> None:
		"""定时器回调：驱动一次 on_update，完成则结束阻塞。"""
		try:
			finished = self.on_update()
		except Exception:
			# 出现异常时也要退出阻塞，避免对话框永久卡死。
			self.accept()
			raise
		if finished:
			self.accept()
		pass

	def _set_title(self, title: str) -> None:
		"""设置第一行左侧的任务标题。"""
		self._title_label.setText(title)
		self.setWindowTitle(title)
		pass

	def _set_detail_message(self, detail: str) -> None:
		"""设置第三行的当前子任务描述。"""
		self._detail_label.setText(detail)
		self._process_qt_events()
		pass

	def _set_total(self, total: int) -> None:
		"""设置进度条的最大值。"""
		self._total = max(1, int(total))
		self._progress_bar.setRange(0, self._total)
		self._set_progress(min(self._value, self._total))
		pass

	def _set_progress(self, value: int | float, detail: str = "") -> None:
		"""设置当前进度值，并刷新百分比与进度条。"""
		self._value = max(0, min(int(value), self._total))
		self._progress_bar.setValue(self._value)
		percent = int(round(self._value * 100.0 / float(self._total)))
		self._percent_label.setText(f"{percent}%")
		if detail is not None:
			self._detail_label.setText(detail)
		self._process_qt_events()
		pass

	@staticmethod
	def _process_qt_events() -> None:
		"""强制刷新界面，保证阻塞期间进度可见。"""
		app = QApplication.instance()
		if app is not None:
			app.processEvents()
		pass
