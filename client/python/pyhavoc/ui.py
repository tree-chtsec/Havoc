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

    _hc_name: str

    def __init__( self ):
        print( f"[HcListenerView] __init__()" )

    ##
    ## main entrypoint what the
    ## Havoc client is going to call
    ##
    def _hc_main( self ):
        print( f"[HcListenerView] _hc_main()" )
        self.main()

    ##
    ## set the protocol name
    ##
    def _hc_set_name(self, name: str ):
        self._hc_name = name
        print( f"[HcListenerView] _hc_name -> {self._hc_name}" )

    ##
    ## get our protocol widget
    ##
    def listener_widget( self ) -> QtWidgets.QWidget:

        widget = HcUiGetWidgetByObjectName( "HcListenerDialog.Protocol." + self._hc_name )

        if widget is None:
            raise "HcListenerDialog protocol widget not found: " + "HcListenerDialog.Protocol." + self._hc_name

        return widget

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
