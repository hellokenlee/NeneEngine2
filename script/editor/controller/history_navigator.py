# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

import os


class HistoryNavigator(object):
	"""线性历史 + 指针模型"""

	def __init__(self, start: str):
		self._history: list[str] = [start]
		self._i: int = 0

	def current(self) -> str:
		return self._history[self._i]

	def split(self) -> list[str]:
		return self.current().split(os.path.sep)

	def can_back(self) -> bool:
		return self._i > 0

	def can_forward(self) -> bool:
		return self._i < len(self._history) - 1

	def push(self, path: str):
		# 若指针不在末尾，截断右侧历史
		if self._i < len(self._history) - 1:
			self._history = self._history[: self._i + 1]
		self._history.append(path)
		self._i += 1

	def back(self) -> str | None:
		if not self.can_back():
			return None
		self._i -= 1
		return self.current()

	def forward(self) -> str | None:
		if not self.can_forward():
			return None
		self._i += 1
		return self.current()
