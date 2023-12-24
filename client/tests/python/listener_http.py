import pyhavoc

import PySide6
from PySide6 import QtWidgets

@pyhavoc.ui.HcUiListenerRegisterView( "Http" )
class HcListenerHttp( pyhavoc.ui.HcListenerView ):

    ##
    ## base widgets
    ##
    layout       : QtWidgets.QGridLayout = None
    tab_widget   : QtWidgets.QTabWidget  = None

    ##
    ## page widgets
    ##
    page_option  : QtWidgets.QWidget     = None
    page_proxy   : QtWidgets.QWidget     = None
    page_server  : QtWidgets.QWidget     = None
    page_client  : QtWidgets.QWidget     = None
    page_preview : QtWidgets.QWidget     = None
    page_help    : QtWidgets.QWidget     = None

    ##
    ## page options widgets
    ##
    opt_layout         : QtWidgets.QGridLayout = None
    opt_label_hostbind : QtWidgets.QLabel      = None
    opt_combo_hostbind : QtWidgets.QComboBox   = None
    opt_label_port     : QtWidgets.QLabel      = None
    opt_input_port     : QtWidgets.QLineEdit   = None

    ##
    ## page
    ##

    ##
    ## main function to create and
    ## render the widgets for the listener
    ##
    def main( self ):

        widget = self.listener_widget()

        ##
        ## create base widget
        ##
        self.layout = QtWidgets.QGridLayout( widget )
        self.layout.setContentsMargins( 0, 0, 0, 0 )

        self.tab_widget = QtWidgets.QTabWidget()
        self.tab_widget.tabBar().setProperty( "HcProtocolTab", "true" )
        self.tab_widget.tabBar().setExpanding( True )
        self.tab_widget.tabBar().setDocumentMode( True )

        self.layout.addWidget( self.tab_widget, 0, 0, 1, 1 )

        ##
        ## add pages
        ##
        self.option_create()
        self.server_create()
        self.client_create()
        self.preview_create()
        self.help_create()

        return

    def option_create( self ):

        self.page_option = QtWidgets.QWidget()
        self.page_option.setProperty( "HcWidgetDark", "true" )

        self.opt_layout  = QtWidgets.QGridLayout( self.page_option )

        self.opt_label_hostbind = QtWidgets.QLabel( "Host (Bind): " )
        self.opt_combo_hostbind = QtWidgets.QComboBox()
        self.opt_combo_hostbind.addItem( "0.0.0.0"   )
        self.opt_combo_hostbind.addItem( "127.0.0.1" )

        self.opt_label_port = QtWidgets.QLabel( "Port: " )
        self.opt_input_port = QtWidgets.QLineEdit()

        self.opt_layout.addWidget( self.opt_label_hostbind, 0, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_combo_hostbind, 0, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_port,     1, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_input_port,     1, 1, 1, 1 )

        self.tab_widget.addTab( self.page_option, "Options" )

        return

    def proxy_create( self ):

        self.page_proxy = QtWidgets.QWidget()

        self.tab_widget.addTab( self.page_proxy, "Proxy" )

        return

    def server_create( self ):

        self.page_server = QtWidgets.QWidget()

        self.tab_widget.addTab( self.page_server, "Server" )

        return

    def client_create( self ):

        self.page_client = QtWidgets.QWidget()

        self.tab_widget.addTab( self.page_client, "Client" )

        return

    def preview_create( self ):

        self.page_preview = QtWidgets.QWidget()

        self.tab_widget.addTab( self.page_preview, "Preview" )

        return

    def help_create( self ):

        self.page_help = QtWidgets.QWidget()

        self.tab_widget.addTab( self.page_help, "Help" )

        return

    ##
    ## pressing "save" action
    ##
    def save( self ) -> dict:
        print( f"[HcListenerHttp] hello from save()" )
        return