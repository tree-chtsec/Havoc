import pyhavoc

from PySide6.QtCore import *  
from PySide6.QtGui import *  
from PySide6.QtWidgets import *

class HcWidgetFile( QWidget ):

    PathMaxLen: int = 40

    def __init__( self, *args, **kwargs ):
        super().__init__( *args, **kwargs )

        buttonSizeMax = QSize( 70, 16777215 )
        buttonSizeMin = QSize( 70, 0 )

        self.file_path: str = ""

        self.setContentsMargins( 0, 0, 0, 0 )
        self.setObjectName( u"HcWidgetFile" )

        self.gridLayout = QGridLayout( self )
        self.gridLayout.setObjectName( u"gridLayout" )
        self.gridLayout.setContentsMargins( 0, 0, 0, 0 )

        self.Label = QLabel( self )
        self.Label.setObjectName( u"Label" )
        self.Label.setProperty( "HcLabelDisplay", "true" )
        self.Label.setText( u"(empty)" )

        self.ButtonLoad = QPushButton( self )
        self.ButtonLoad.setObjectName( u"ButtonLoad" )
        self.ButtonLoad.setText( u"Load" )
        self.ButtonLoad.setMaximumSize( buttonSizeMax )
        self.ButtonLoad.setMinimumSize( buttonSizeMin )
        self.ButtonLoad.setProperty( "HcButton", "true" )
        self.ButtonLoad.clicked.connect( self.button_load )

        self.ButtonRemove = QPushButton( self )
        self.ButtonRemove.setObjectName( u"ButtonRemove" )
        self.ButtonRemove.setText( u"Remove" )
        self.ButtonRemove.setMaximumSize( buttonSizeMax )
        self.ButtonRemove.setMinimumSize( buttonSizeMin )
        self.ButtonRemove.setProperty( "HcButton", "true" )
        self.ButtonRemove.clicked.connect( self.button_remove )

        self.gridLayout.addWidget( self.Label,        0, 0, 1, 1 )
        self.gridLayout.addWidget( self.ButtonLoad,   0, 1, 1, 1 )
        self.gridLayout.addWidget( self.ButtonRemove, 0, 2, 1, 1 )

        QMetaObject.connectSlotsByName( self )

        return

    def button_load( self ) -> None:
        file_dialog = QFileDialog()
        file_dialog.setStyleSheet( pyhavoc.ui.HcUiGetStyleSheet() )

        if file_dialog.exec() == QDialog.Accepted:
            self.file_path = file_dialog.selectedFiles()[ 0 ]

            path = ""

            if len( self.file_path ) > self.PathMaxLen:
                path = "..."
                path = path + self.file_path[ len( self.file_path ) - self.PathMaxLen : len( self.file_path ) ]
            else:
                path = self.file_path

            self.Label.setText( path )

        file_dialog.deleteLater()

        return

    def button_remove( self ) -> None:

        self.file_path = ""
        self.Label.setText( "(empty)" )

        return


class HcWidgetList( QWidget ):

    def __init__( self, *args, **kwargs ):
        super().__init__( *args, **kwargs )

        button_size = QSize( 100, 25 )

        self.setObjectName( u"HcWidgetList" )

        self.GridLayout = QGridLayout( self )
        self.GridLayout.setObjectName( u"gridLayout" )
        self.GridLayout.setContentsMargins( 0, 0, 0, 0 )

        self.GroupList = QGroupBox( self )
        self.GroupList.setObjectName( u"GroupList" )
        self.GroupList.setTitle( u"" )
        self.FormLayout = QFormLayout( self.GroupList )

        self.ButtonAdd = QPushButton( self )
        self.ButtonAdd.setObjectName( u"ButtonAdd" )
        self.ButtonAdd.setText( u"Add" )
        self.ButtonAdd.setProperty( "HcButton", "true" )
        self.ButtonAdd.setMaximumSize( button_size )
        self.ButtonAdd.setMinimumSize( button_size )
        self.ButtonAdd.setText( u"Add" )
        self.ButtonAdd.clicked.connect( self.button_add )

        self.ButtonClear = QPushButton( self )
        self.ButtonClear.setObjectName( u"ButtonClear" )
        self.ButtonClear.setProperty( "HcButton", "true" )
        self.ButtonClear.setMaximumSize( button_size )
        self.ButtonClear.setMinimumSize( button_size )
        self.ButtonClear.setText( u"Clear" )
        self.ButtonClear.clicked.connect( self.button_clear )

        self.GridLayout.addWidget( self.GroupList,   0, 0, 3, 1 )
        self.GridLayout.addWidget( self.ButtonAdd,   0, 1, 1, 1 )
        self.GridLayout.addWidget( self.ButtonClear, 1, 1, 1, 1 )

        self.InputList : list[QLineEdit] = []

        QMetaObject.connectSlotsByName( self )

        return

    def add_input( self, value: str = "" ) -> None:
        line_edit = QLineEdit()
        line_edit.setFocus()
        line_edit.setText( value )

        self.FormLayout.setWidget( len( self.InputList ), QFormLayout.FieldRole, line_edit )

        self.InputList.append( line_edit )

        return

    def button_add( self ) -> None:
        self.add_input()
        return

    def button_clear( self ) -> None:

        for i in self.InputList:
            i.deleteLater()

        self.InputList = []
        return

@pyhavoc.ui.HcUiListenerRegisterView( "Http" )
class HcListenerHttp( pyhavoc.ui.HcListenerView ):

    def __init__( self, *args, **kwargs ):
        super().__init__( *args, **kwargs )

        ##
        ## base widgets
        ##
        self.layout     : QGridLayout = None
        self.tab_widget : QTabWidget  = None

        ##
        ## page widgets
        ##
        self.page_option  : QWidget = None
        self.page_proxy   : QWidget = None
        self.page_server  : QWidget = None
        self.page_client  : QWidget = None
        self.page_preview : QWidget = None
        self.page_help    : QWidget = None

        ##
        ## page options widgets
        ##
        self.opt_layout            : QGridLayout  = None
        self.opt_label_hostbind    : QLabel       = None
        self.opt_combo_hostbind    : QComboBox    = None
        self.opt_label_port        : QLabel       = None
        self.opt_input_port        : QLineEdit    = None
        self.opt_label_hosts       : QLabel       = None
        self.opt_list_hosts        : HcWidgetList = None
        self.opt_space_hosts       : QSpacerItem  = None

        self.opt_label_rotation    : QLabel       = None
        self.opt_combo_rotation    : QComboBox    = None
        self.opt_label_useragent   : QLabel       = None
        self.opt_input_useragent   : QLineEdit    = None
        self.opt_label_uris        : QLabel       = None
        self.opt_list_uris         : HcWidgetList = None
        self.opt_space_uris        : QSpacerItem  = None
        self.opt_label_hostheader  : QLabel       = None
        self.opt_input_hostheader  : QLineEdit    = None
        self.opt_label_secure      : QLabel       = None
        self.opt_combo_secure      : QComboBox    = None
        self.opt_label_server_cert : QLabel       = None
        self.opt_file_server_cert  : HcWidgetFile = None
        self.opt_label_server_key  : QLabel       = None
        self.opt_file_server_key   : HcWidgetFile = None

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
        self.proxy_create()
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

        self.opt_label_hosts = QLabel( "Hosts: " )
        self.opt_list_hosts  = HcWidgetList()
        self.opt_space_hosts = QSpacerItem( 20, 125, QSizePolicy.Minimum, QSizePolicy.Expanding )

        self.opt_label_uris = QLabel( "Uris: " )
        self.opt_list_uris  = HcWidgetList()
        self.opt_space_uris = QSpacerItem( 20, 125, QSizePolicy.Minimum, QSizePolicy.Expanding )

        self.opt_label_rotation = QLabel( "Host Rotation: " )
        self.opt_combo_rotation = QComboBox()
        self.opt_combo_rotation.addItem( "round-robin" )
        self.opt_combo_rotation.addItem( "random" )

        self.opt_label_useragent = QLabel( "User Agent: " )
        self.opt_input_useragent = QLineEdit()

        self.opt_label_hostheader = QLabel( "Host Header: " )
        self.opt_input_hostheader = QLineEdit()

        self.opt_label_secure = QLabel( "Secure: " )
        self.opt_combo_secure = QComboBox()
        self.opt_combo_secure.addItem( "true" )
        self.opt_combo_secure.addItem( "false" )

        self.opt_label_server_cert = QLabel( "Server Cert: " )
        self.opt_file_server_cert  = HcWidgetFile()
        self.opt_label_server_key  = QLabel( "Server Key : " )
        self.opt_file_server_key   = HcWidgetFile()

        self.opt_layout.addWidget( self.opt_label_hostbind,    0, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_combo_hostbind,    0, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_port,        1, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_input_port,        1, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_hosts,       2, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_list_hosts,        2, 1, 2, 1 )
        self.opt_layout.addItem( self.opt_space_hosts,         3, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_uris,        4, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_list_uris,         4, 1, 2, 1 )
        self.opt_layout.addItem( self.opt_space_uris,          5, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_rotation,    6, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_combo_rotation,    6, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_useragent,   7, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_input_useragent,   7, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_hostheader,  8, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_input_hostheader,  8, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_secure,      9, 0, 1, 1 )
        self.opt_layout.addWidget( self.opt_combo_secure,      9, 1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_server_cert, 10,0, 1, 1 )
        self.opt_layout.addWidget( self.opt_file_server_cert,  10,1, 1, 1 )
        self.opt_layout.addWidget( self.opt_label_server_key,  11,0, 1, 1 )
        self.opt_layout.addWidget( self.opt_file_server_key,   11,1, 1, 1 )

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