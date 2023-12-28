import pyhavoc

from PySide6.QtCore import *  
from PySide6.QtGui import *  
from PySide6.QtWidgets import *  

@pyhavoc.ui.HcUiListenerRegisterView( "Http" )
class HcListenerHttp( pyhavoc.ui.HcListenerView ):

    ##
    ## base widgets
    ##
    layout       : QGridLayout = None
    tab_widget   : QTabWidget  = None

    ##
    ## page widgets
    ##
    page_option  : QWidget     = None
    page_proxy   : QWidget     = None
    page_server  : QWidget     = None
    page_client  : QWidget     = None
    page_preview : QWidget     = None
    page_help    : QWidget     = None

    ##
    ## page options widgets
    ##
    opt_layout         : QGridLayout = None
    opt_label_hostbind : QLabel      = None
    opt_combo_hostbind : QComboBox   = None
    opt_label_port     : QLabel      = None
    opt_input_port     : QLineEdit   = None

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
        self.layout = QGridLayout( widget )
        self.layout.setContentsMargins( 0, 0, 0, 0 )

        self.tab_widget = QTabWidget()
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

        self.page_option = QWidget()
        self.page_option.setProperty( "HcWidgetDark", "true" )

        self.opt_layout  = QGridLayout( self.page_option )

        self.opt_label_hostbind = QLabel( "Host (Bind): " )
        self.opt_combo_hostbind = QComboBox()
        self.opt_combo_hostbind.addItem( "0.0.0.0"   )
        self.opt_combo_hostbind.addItem( "127.0.0.1" )

        self.opt_label_port = QLabel( "Port: " )
        self.opt_input_port = QLineEdit()

        self.opt_layout.addWidget( self.opt_label_hostbind, 0, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_combo_hostbind, 0, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_port,     1, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_input_port,     1, 1, 1, 1 )

        self.tab_widget.addTab( self.page_option, "Options" )

        return

    def proxy_create( self ):

        self.page_proxy = QWidget()

        self.tab_widget.addTab( self.page_proxy, "Proxy" )

        return

    def server_create( self ):

        self.page_server = QWidget()

        self.tab_widget.addTab( self.page_server, "Server" )

        return

    def client_create( self ):

        self.page_client = QWidget()

        self.tab_widget.addTab( self.page_client, "Client" )

        return

    def preview_create( self ):

        self.page_preview = QWidget()

        self.tab_widget.addTab( self.page_preview, "Preview" )

        return

    def help_create( self ):

        self.page_help = QWidget()

        self.tab_widget.addTab( self.page_help, "Help" )

        return

    ##
    ## pressing "save" action
    ##
    def save( self ) -> dict:
        print( f"[HcListenerHttp] hello from save()" )
        return