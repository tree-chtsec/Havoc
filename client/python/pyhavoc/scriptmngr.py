from _pyhavoc import core

import sys
import os
import importlib.util

class HcPyScriptMngrThreadManager:

    def __init__(self):
        pass

class HcPyScriptMngrStdOutErrHandler:
    def __init__( self ):
        return

    def write( self, data ):
        core.HcScriptManagerConsoleStdOut( data )

@core.HcScriptManagerLoadScriptCallback
def _HcPyScriptLoad(
    _script_path: str
) -> None:

    script_name = os.path.splitext( os.path.basename( _script_path ) )[ 0 ]
    module_name = "HcScript." + script_name
    script_spec = importlib.util.spec_from_file_location( module_name, _script_path )
    module      = importlib.util.module_from_spec( script_spec )

    script_spec.loader.exec_module( module )

    return

sys.stdout = HcPyScriptMngrStdOutErrHandler()
sys.stderr = HcPyScriptMngrStdOutErrHandler()