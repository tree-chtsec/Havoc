##
## import havoc specific libs
##
from _pyhavoc import core
from _pyhavoc import ui

##
## import qt ui library
##
import PySide6
from PySide6 import QtWidgets

def HcUiGetWidgetByObjectName(
    object_name: str
) -> QtWidgets.QWidget:

    for widget in QtWidgets.QApplication.instance().allWidgets():
        if str( widget.objectName() ) == object_name:
            return widget

    return None

class HcListenerView:

    def __init__( self ):
        pass

    ##
    ## main entrypoint what the
    ## Havoc client is going to call
    ##
    def _main( self ):
        self.main()

    def listener_widget( self ) -> QtWidgets.QWidget:
        pass

    ##
    ## main function to be executed
    ## should create the widgets inputs
    ## for the listener
    ##
    def main( self ):
        pass

    ##
    ## pressing "save" action
    ##
    def save( self ) -> dict:
        pass

def HcUiPayloadBuilderObjName() -> str:
    return ui.HcUiPayloadBuilderObjName()

def HcUiListenerObjName() -> str:
    return ui.HcUiListenerObjName()

def HcUiListenerRegisterView( protocol: str ):

    def _register( listener_view ):
        ui.HcUiListenerRegisterView( protocol, listener_view )

    return _register
