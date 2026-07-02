
from typing import TypeVar

from PySide6.QtCore import QObject

TChildWidget = TypeVar("TChildWidget", bound=QObject)
class BaseWidget(object):
	"""
		Helper class for widgets.
		Multi-inherit this class.
	"""

	UI_FILE = ""

	def __init__(self):
		super().__init__()
		self._has_set_up = False
		pass

	def setup(self):
		assert not self._has_set_up, "You cannot setup a widget (%s) twice." % self.__class__.__name__
		self._has_set_up = True
		pass

	def find_child(self, child_class: type[TChildWidget], child_name: str = "") -> TChildWidget:
		assert isinstance(self, QObject), (
			"The `WidgetWrapper` can only be used on `QObject`, "
			"but the current object is `%s`." % type(self).__name__
		)
		child_widget = self.findChild(child_class, child_name)
		assert child_widget is not None, "Cannot find child widget: %s (%s)" % (child_name, child_class.__name__)
		return child_widget
