##
## import havoc specific libs
##
from _pyhavoc import core

import PySide6
from PySide6 import QtCore
from PySide6 import QtWidgets

def HcUiGetWidgetByObjectName(
    object_name: str
) -> PySide6.QtWidgets.QWidget:

    for widget in QtWidgets.QApplication.instance().allWidgets():
        if str( widget.objectName() ) == object_name:
            return widget

    return None

def HcUiPayloadBuilderWidgetName() -> str:
    return core.HcUiPayloadBuilderWidgetName()