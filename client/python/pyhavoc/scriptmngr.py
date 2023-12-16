from _pyhavoc import core
import time

def _HcScriptMngrHandleInput() -> None:
    core.HcScriptManagerConsoleStdOut( f"[_HcScriptMngrHandleInput] text ->" )
    return

core.HcScriptManagerConsoleStdOut( "[Havoc Python Console]" )
core.HcScriptManagerConsoleStdIn( _HcScriptMngrHandleInput )
