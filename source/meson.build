# -*- coding=utf-8 -*-
# __author__ = "KenLee"
# __email__ = "hellokenlee@163.com"
#
# Engine modules, declared in dependency (topological) order so that every
# `subdir()` can reference the `<module>_dep` defined by the previous ones
# (meson `subdir()` shares the current scope).

subdir('core')
subdir('gapi')
subdir('gapi_d3d12')
subdir('gapi_vulkan')
subdir('gapi_dynamic')
subdir('core_render')
subdir('core_object')
subdir('renderer')
subdir('engine')
subdir('app')
subdir('editor')

# `shader` is a content-only module (legacy BuildTarget.NONE): nothing to build.
