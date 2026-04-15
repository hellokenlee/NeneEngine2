# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"

from typing import Generic, TypeVar
from PySide6 import QtCore
from PySide6.QtWidgets import QWidget, QLabel, QHBoxLayout, QDoubleSpinBox, QSpinBox, QSizePolicy, QApplication, QLineEdit

from nene import Float3, Rotator, AssetHandle, AssetRegistry
from script.editor.widget.content_broswer_view_widget import ContentBrowserViewWidget

T = TypeVar("T")


class PropertyEdit(object):

	def __init__(self, _component: object, _prop_name: str):
		super().__init__()
		pass


class MutablePropertyEdit(PropertyEdit, Generic[T]):

	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		self.data: T = getattr(component, prop_name)
		pass


class ImmutablePropertyEdit(PropertyEdit, Generic[T]):
	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		self.data: T = getattr(component, prop_name)
		self.component = component
		self.prop_name = prop_name
		pass

	def on_value_change(self, value: T):
		self.data = value
		setattr(self.component, self.prop_name, value)
		pass


# noinspection DuplicatedCode
class _AxisDoubleSpinBox(QDoubleSpinBox):
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


# noinspection DuplicatedCode
class _AxisIntegerSpinBox(QSpinBox):
	def __init__(self, color_hex: str, parent=None):
		super().__init__(parent)
		self.setRange(-999999, 999999)
		self.setSingleStep(1)
		self.setSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
		self.setFixedWidth(52)
		self.setButtonSymbols(QSpinBox.ButtonSymbols.NoButtons)
		self.setStyleSheet(
			"QSpinBox{padding-left:2px;}"
			"QSpinBox::up-button,QSpinBox::down-button{width:0px;}"
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


class IntegerWidget(QWidget):

	def __init__(self):
		super().__init__()
		#
		layout = QHBoxLayout(self)
		layout.setContentsMargins(4, 0, 0, 0)
		layout.setSpacing(4)
		#
		self._spin = _AxisIntegerSpinBox("#999999", self)
		layout.addWidget(self._spin)
		#
		self._spin.blockSignals(True)
		self._spin.blockSignals(False)
		#
		self.setFocusProxy(self._spin)
		pass


class IntegerPropertyWidget(ImmutablePropertyEdit[int], IntegerWidget):
	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		self._spin.setValue(self.data)
		self._spin.valueChanged.connect(self.on_value_change)
		pass


class FloatWidget(QWidget):

	def __init__(self):
		super().__init__()
		#
		layout = QHBoxLayout(self)
		layout.setContentsMargins(4, 0, 0, 0)
		layout.setSpacing(4)
		#
		self._spin = _AxisDoubleSpinBox("#999999", self)
		self._spin.setDecimals(3)
		layout.addWidget(self._spin)
		#
		self._spin.blockSignals(True)
		self._spin.blockSignals(False)
		#
		self.setFocusProxy(self._spin)
		pass


class FloatPropertyWidget(ImmutablePropertyEdit[float], FloatWidget):

	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		self._spin.setValue(self.data)
		self._spin.valueChanged.connect(self.on_value_change)
		pass


class RGBWidget(QWidget):

	def __init__(self):
		super().__init__()
		#
		layout = QHBoxLayout(self)
		layout.setContentsMargins(4, 0, 0, 0)
		layout.setSpacing(4)
		#
		self._r_spin = _AxisDoubleSpinBox("#df5a4f", self)
		self._g_spin = _AxisDoubleSpinBox("#6dbb4a", self)
		self._b_spin = _AxisDoubleSpinBox("#4a84d8", self)
		layout.addWidget(self._r_spin)
		layout.addWidget(self._g_spin)
		layout.addWidget(self._b_spin)
		#
		self._r_spin.valueChanged.connect(self.on_value_change)
		self._g_spin.valueChanged.connect(self.on_value_change)
		self._b_spin.valueChanged.connect(self.on_value_change)
		
		# Set the focus proxy to improve focus behavior when clicking on the RGBWidget itself.
		self.setFocusProxy(self._r_spin)
		pass

	def on_value_change(self):
		pass

	def focusNextPrevChild(self, next_prev: bool) -> bool:
		"""
		Custom focus navigation to cycle between the three spin boxes when Tab/Backtab is pressed.
		"""
		# Get a currently focused widget from the application
		focused = QApplication.focusWidget()
		spins = [self._r_spin, self._g_spin, self._b_spin]
		
		# Identify which spin box (if any) currently has focus (including its internal lineEdit)
		current_spin = None
		for spin in spins:
			# noinspection PyTypeChecker
			if focused == spin or spin.isAncestorOf(focused):
				current_spin = spin
				break
		
		# If focus is within one of our spin boxes, cycle between them
		if current_spin:
			idx = spins.index(current_spin)
			# Tab
			if next_prev:
				target = spins[(idx + 1) % 3]
			# Backtab
			else:
				target = spins[(idx - 1) % 3]
			target.setFocus(QtCore.Qt.FocusReason.TabFocusReason)
			# Ensure text is selected for easier editing
			if hasattr(target, 'lineEdit'):
				target.lineEdit().selectAll()
			return True
		return super().focusNextPrevChild(next_prev)


class Float3PropertyWidget(MutablePropertyEdit[Float3], RGBWidget):

	def __init__(self, component: object, prop_name: str):
		#
		super().__init__(component, prop_name)
		#
		for spin, val in ((self._r_spin, self.data.x), (self._g_spin, self.data.y), (self._b_spin, self.data.z)):
			spin.blockSignals(True)
			spin.setValue(float(val))
			spin.blockSignals(False)
		pass

	def on_value_change(self):
		self.data.x = self._r_spin.value()
		self.data.y = self._g_spin.value()
		self.data.z = self._b_spin.value()
		pass


class RotatorPropertyWidget(MutablePropertyEdit[Rotator], RGBWidget):

	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		#
		for spin, val in ((self._r_spin, self.data.pitch), (self._g_spin, self.data.roll), (self._b_spin, self.data.yaw)):
			spin.blockSignals(True)
			spin.setValue(float(val))
			spin.blockSignals(False)
		pass

	def on_value_change(self):
		self.data.pitch = self._r_spin.value()
		self.data.roll = self._g_spin.value()
		self.data.yaw = self._b_spin.value()
		pass


class AssetHandlePropertyWidget(MutablePropertyEdit[AssetHandle], QWidget):

	def __init__(self, component: object, prop_name: str):
		super().__init__(component, prop_name)
		self.setAcceptDrops(True)
		#
		layout = QHBoxLayout(self)
		layout.setContentsMargins(4, 0, 0, 0)
		layout.setSpacing(4)
		#
		self._line_edit = QLineEdit(self)
		self._line_edit.setReadOnly(True)
		self._line_edit.setPlaceholderText("None")
		self._update_display()
		layout.addWidget(self._line_edit)
		pass

	def _resolve_file_name(self) -> str:
		if self.data.m_uuid is None:
			return ""
		abstract = AssetRegistry().find_abstract_by_uuid(self.data.m_uuid)
		file_name = abstract.m_file_name
		if not file_name:
			return ""
		import os
		return os.path.basename(file_name)

	def _resolve_full_path(self) -> str:
		if self.data.m_uuid is None:
			return ""
		abstract = AssetRegistry().find_abstract_by_uuid(self.data.m_uuid)
		file_name = abstract.m_file_name
		return file_name if file_name else ""

	def _resolve_uuid_str(self) -> str:
		if self.data.m_uuid is None:
			return ""
		return str(self.data.m_uuid)

	def _update_display(self):
		self._line_edit.setText(self._resolve_file_name())
		uid = self._resolve_uuid_str()
		path = self._resolve_full_path()
		if uid or path:
			self._line_edit.setToolTip(f"uuid: {uid}\npath: {path}")
		else:
			self._line_edit.setToolTip("")

	def dragEnterEvent(self, event):
		if event.mimeData().hasFormat(ContentBrowserViewWidget.ASSET_MIME_TYPE):
			event.acceptProposedAction()

	def dropEvent(self, event):
		if event.mimeData().hasFormat(ContentBrowserViewWidget.ASSET_MIME_TYPE):
			raw = bytes(event.mimeData().data(ContentBrowserViewWidget.ASSET_MIME_TYPE).data()).decode("utf-8")
			paths = raw.split("\n")
			if paths:
				path = paths[0].strip()
				abstract = AssetRegistry().find_abstract(path)
				if abstract.m_uuid is not None:
					self.data.m_uuid = abstract.m_uuid
					self._update_display()
			event.acceptProposedAction()
		pass
