
from enum import Enum


"""
EscapeRule:
    [ret: bit] [action: bit[4]]

"""


class Action(Enum):
    NoAction = 0
    CursorUp = 1
    CursorDown = 2
    CursorRight = 3
    CursorLeft = 4
    DeleteBefore = 5
    DeleteAfter = 6
    Execute = 7
    Skip = 8
    Fallback = 9 # print full escape
    Insert = 10


NoAction     = Action.NoAction    
CursorUp     = Action.CursorUp    
CursorDown   = Action.CursorDown  
CursorRight  = Action.CursorRight 
CursorLeft   = Action.CursorLeft  
DeleteBefore = Action.DeleteBefore
DeleteAfter  = Action.DeleteAfter 
Execute      = Action.Execute     
Skip         = Action.Skip     
Fallback     = Action.Fallback        
Insert       = Action.Insert
