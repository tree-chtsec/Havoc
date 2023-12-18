from _pyhavoc import core

import sys

class HcPyScriptMngrThreadManager:

    def __init__(self):
        pass

class HcPyScriptMngrStdOutErrHandler:
    def __init__( self ):
        return

    def write( self, data ):
        core.HcScriptManagerConsoleStdOut( data )

sys.stdout = HcPyScriptMngrStdOutErrHandler()
sys.stderr = HcPyScriptMngrStdOutErrHandler()