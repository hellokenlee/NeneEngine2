# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from PySide6 import QtCore
from PySide6.QtCore import QSize
from PySide6.QtWidgets import QWidget, QLabel, QHBoxLayout, QVBoxLayout, QDoubleSpinBox, QSizePolicy, QToolButton, QFrame

from script.editor.resource_set import IconSet


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


class Float3Widget(QWidget):
	value_changed = QtCore.Signal(float, float, float)

	def __init__(self, display_name: str = "", parent=None):
		super().__init__(parent)
		layout = QHBoxLayout(self)
		layout.setContentsMargins(0, 0, 0, 0)
		layout.setSpacing(4)

		self._name_label = QLabel(display_name, self)
		self._name_label.setMinimumWidth(56)
		layout.addWidget(self._name_label)

		self._r_spin = _AxisSpinBox("#df5a4f", self)
		self._g_spin = _AxisSpinBox("#6dbb4a", self)
		self._b_spin = _AxisSpinBox("#4a84d8", self)
		layout.addWidget(self._r_spin)
		layout.addWidget(self._g_spin)
		layout.addWidget(self._b_spin)
		layout.addStretch(1)

		self._r_spin.valueChanged.connect(self._emit_value_changed)
		self._g_spin.valueChanged.connect(self._emit_value_changed)
		self._b_spin.valueChanged.connect(self._emit_value_changed)
		pass

	def set_display_name(self, text: str):
		self._name_label.setText(text)
		pass

	def set_value(self, x: float, y: float, z: float):
		for spin, val in ((self._r_spin, x), (self._g_spin, y), (self._b_spin, z)):
			spin.blockSignals(True)
			spin.setValue(float(val))
			spin.blockSignals(False)
		pass

	def _emit_value_changed(self):
		self.value_changed.emit(self._r_spin.value(), self._g_spin.value(), self._b_spin.value())
		pass


class ComponentDetailWidget(QWidget):
	"""Inspector 分组：带标题、可折叠、支持动态增删 PropertyWidget。"""

	def __init__(self, title: str = "", parent=None, collapsed: bool = False):
		super().__init__(parent)
		self._slots: list[QWidget | None] = []

		root_layout = QVBoxLayout(self)
		root_layout.setContentsMargins(0, 0, 0, 0)
		root_layout.setSpacing(0)

		self._toggle_btn = QToolButton(self)
		self._toggle_btn.setText(title)
		self._toggle_btn.setCheckable(True)
		self._toggle_btn.setChecked(not collapsed)
		self._toggle_btn.setArrowType(QtCore.Qt.ArrowType.NoArrow)
		self._toggle_btn.setToolButtonStyle(QtCore.Qt.ToolButtonStyle.ToolButtonTextBesideIcon)
		self._toggle_btn.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
		font = self._toggle_btn.font()
		font.setBold(True)
		self._toggle_btn.setFont(font)
		self._toggle_btn.setIconSize(QSize(10, 10))
		self._toggle_btn.setAutoRaise(True)
		self._toggle_btn.setStyleSheet(
			"QToolButton {"
			"text-align: left;"
			"background: transparent;"
			"border: none;"
			"outline: none;"
			"color: palette(button-text);"
			"}"
			"QToolButton:hover {"
			"background: transparent;"
			"border: none;"
			"color: palette(button-text);"
			"}"
			"QToolButton:pressed {"
			"background: transparent;"
			"border: none;"
			"color: palette(button-text);"
			"}"
			"QToolButton:checked {"
			"background: transparent;"
			"border: none;"
			"color: palette(button-text);"
			"}"
			"QToolButton:focus {"
			"background: transparent;"
			"border: none;"
			"color: palette(button-text);"
			"}"
		)
		self._update_toggle_icon(collapsed)
		self._toggle_btn.clicked.connect(self._on_toggle_clicked)
		root_layout.addWidget(self._toggle_btn)

		self._content_frame = QFrame(self)
		self._content_layout = QVBoxLayout(self._content_frame)
		self._content_layout.setContentsMargins(17, 4, 0, 4)
		self._content_layout.setSpacing(4)
		root_layout.addWidget(self._content_frame)
		self._content_frame.setVisible(not collapsed)
		pass

	def title(self) -> str:
		return self._toggle_btn.text()

	def set_title(self, title: str):
		self._toggle_btn.setText(title)
		pass

	def is_collapsed(self) -> bool:
		return not self._content_frame.isVisible()

	def set_collapsed(self, collapsed: bool):
		self._content_frame.setVisible(not collapsed)
		self._toggle_btn.setChecked(not collapsed)
		self._update_toggle_icon(collapsed)
		pass

	def add_property_widget(self, widget: QWidget):
		"""追加一个 PropertyWidget。"""
		self._slots.append(widget)
		self._content_layout.addWidget(widget)
		pass

	def reserve_slots(self, slot_count: int):
		"""预留空槽位，后续可通过 set_property_widget 动态填充。"""
		if slot_count <= len(self._slots):
			return
		for _ in range(slot_count - len(self._slots)):
			self._slots.append(None)
		pass

	def set_property_widget(self, index: int, widget: QWidget):
		"""设置指定槽位控件；若原槽位已有控件会被替换。"""
		if index < 0:
			raise IndexError(index)
		self.reserve_slots(index + 1)

		old_widget = self._slots[index]
		if old_widget is not None:
			self._content_layout.removeWidget(old_widget)
			old_widget.deleteLater()

		self._slots[index] = widget

		insert_pos = 0
		for i in range(index):
			if self._slots[i] is not None:
				insert_pos += 1
		self._content_layout.insertWidget(insert_pos, widget)
		pass

	def clear_property_widgets(self):
		for widget in self._slots:
			if widget is not None:
				self._content_layout.removeWidget(widget)
				widget.deleteLater()
		self._slots.clear()
		pass

	def _on_toggle_clicked(self, checked: bool):
		self._content_frame.setVisible(checked)
		self._update_toggle_icon(not checked)
		pass

	def _update_toggle_icon(self, collapsed: bool):
		if collapsed:
			self._toggle_btn.setIcon(IconSet().right_triangle)
			self._toggle_btn.setIconSize(QSize(11, 11))
		else:
			self._toggle_btn.setIcon(IconSet().down_triangle)
			self._toggle_btn.setIconSize(QSize(11, 11))
		pass
