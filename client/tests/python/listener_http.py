from pyhavoc import ui as HavocUi

import PySide6
from PySide6 import QtWidgets

@HavocUi.HcUiListenerRegisterView( "Http" )
class HcListenerHttp( HavocUi.HcListenerView ):

    def __init__( self ):
        return

    ##
    ## main function to be executed
    ## should create the widgets inputs
    ## for the listener
    ##
    def main( self ):
        print( f"[{self.__name__}] hello from main()" )
        return

    ##
    ## pressing "save" action
    ##
    def save( self ) -> dict:
        print( f"[{self.__name__}] hello from save()" )
        return