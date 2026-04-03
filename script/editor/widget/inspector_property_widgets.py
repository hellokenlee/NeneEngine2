# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtWidgets import QWidget, QLabel, QHBoxLayout, QDoubleSpinBox, QSizePolicy

from nene import Float3, Rotator


class _AxisSpinBox(QDoubleSpinBox):
	def __init__(self, color_hex: str, parent=None):
		super().__init__(parent)
		self.setDecimals(1)
		self.setRange(-999999.0, 999999.0)
		self.setSingleStep(0.1)
		self.setSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
		# Keep each field compact to avoid forcing horizontal scrolling in inspector.
		self.setFixedWidth(52)
		self.setButtonSymbols(QDoubleSpinBox.ButtonSymbols.NoButtons)
		self.setStyleSheet(
			"QDoubleSpinBox{padding-left:2px;}"
			"QDoubleSpinBox::up-button,QDoubleSpinBox::down-button{width:0px;}"
		)
		self._axis_tag = QLabel("", self)
		self._axis_tag.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
		self._axis_tag.setFixedWidth(2)
		self._axis_tag.setStyleSheet(
			"QLabel{"
			f"background:{color_hex};"
			"color:#111111;"
			"border-radius:2px;"
			"font-weight:700;"
			"}"
		)

	def resizeEvent(self, event):
		super().resizeEvent(event)
		h = self.height() - 6
		if h < 8:
			h = self.height()
		self._axis_tag.setFixedHeight(h)
		self._axis_tag.move(2, (self.height() - h) // 2)
		pass


class RGBWidget(QWidget):

	def __init__(self):
		super().__init__()
		#
		layout = QHBoxLayout(self)
		layout.setContentsMargins(4, 0, 0, 0)
		layout.setSpacing(4)
		#
		self._r_spin = _AxisSpinBox("#df5a4f", self)
		self._g_spin = _AxisSpinBox("#6dbb4a", self)
		self._b_spin = _AxisSpinBox("#4a84d8", self)
		layout.addWidget(self._r_spin)
		layout.addWidget(self._g_spin)
		layout.addWidget(self._b_spin)
		#
		self._r_spin.valueChanged.connect(self._on_value_changed)
		self._g_spin.valueChanged.connect(self._on_value_changed)
		self._b_spin.valueChanged.connect(self._on_value_changed)
		pass

	def _on_value_changed(self):
		pass


class Float3Widget(RGBWidget):

	def __init__(self, data: Float3):
		super().__init__()
		#
		self.data = data
		#
		for spin, val in ((self._r_spin, self.data.x), (self._g_spin, self.data.y), (self._b_spin, self.data.z)):
			spin.blockSignals(True)
			spin.setValue(float(val))
			spin.blockSignals(False)
		pass

	def _on_value_changed(self):
		self.data.x = self._r_spin.value()
		self.data.y = self._g_spin.value()
		self.data.z = self._b_spin.value()
		pass


class RotatorWidget(RGBWidget):

	def __init__(self, data: Rotator):
		super().__init__()
		#
		self.data = data
		#
		for spin, val in ((self._r_spin, self.data.pitch), (self._g_spin, self.data.roll), (self._b_spin, self.data.yaw)):
			spin.blockSignals(True)
			spin.setValue(float(val))
			spin.blockSignals(False)
		pass

	def _on_value_changed(self):
		self.data.pitch = self._r_spin.value()
		self.data.roll = self._g_spin.value()
		self.data.yaw = self._b_spin.value()
		pass