from _pyhavoc import core
import time

def _HcScriptMngrHandleInput(
    text: str
) -> None:
    core.HcScriptManagerConsoleStdOut( f"[_HcScriptMngrHandleInput] text -> {text}" )
    return

core.HcScriptManagerConsoleStdOut( "[Havoc Python Console]" )
core.HcScriptManagerConsoleStdIn( _HcScriptMngrHandleInput )
