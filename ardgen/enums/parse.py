
from enum import Enum
from ardgen import *


"""
/* Rules mapping
 * 
 * Token:
 *   [escape: bit[2]] [token: bit[4]] [state: bit[2]]
 *   token in 0..12 if SHLT_, in 13..15 in SHLT_Escape
 *
 * Key:
 *   [char: bit[7]]
 *
 * Value:
 *   [data: bit[2]] [ret: bit] [0: bit] [0: bit] [switch: bit] [state: bit[2]]
 *   [data: bit[2]] [ret: bit] [1: bit] [token: bit[4]]
 *
 */
#define DataAction(a) ((a) << 6)
#define NoStore DataAction(0)
#define Store8  DataAction(1)
#define Store3  DataAction(2)
#define Store4  DataAction(3)
#define Return  0x20
#define Token(token) (0x10 | (token))
#define State(state, switch) (0x08 | (state) | ((switch) ? 0x4 : 0x0))
"""


class TokenEnum(Enum):
    def __getitem__(self, state: int) -> typing.Callable[[typing.Callable | list], "TokenPair"]:
        return TokenStateFactory(self, state)
    
    def shlt_name(self) -> str:
        name = self.name
        name = name.replace('NotInited', 'None')
        if isinstance(self, Escape):
            return f"SHLT_Escape{name}"
        return f"SHLT_{name}"


@dataclass
class TokenStateFactory:
    token: TokenEnum
    state: int
    
    def link(self, unit: ARDUnit) -> "TokenPairLink":
        return TokenPairLink(unit, self.state, self.token)
    
    def __call__(self, rules) -> "TokenPairList":
        return TokenPairList(rules, state=self.state, token=self.token, description=f"{self.token.__class__.__name__}.{self.token.name}[{self.state}]")


@dataclass
class TokenPair:
    state: int = None
    token: TokenEnum = None


@dataclass
class TokenPairLink(ARDUnit, TokenPair):
    def __init__(self, unit: ARDUnit, state=None, token=None, **kw):
        super().__init__(address=unit.address, **kw)
        TokenPair.__init__(self, state=state, token=token)


@dataclass
class TokenPairList(RulesList, TokenPair):
    def __init__(self, *a, **kw):
        state, token = kw.pop('state'), kw.pop('token')
        super().__init__(*a, **kw)
        TokenPair.__init__(self, state=state, token=token)


class Token(TokenEnum):
    NotInited = 0
    Eof = 1
    StateError = 2
    Whitespace = 3
    FlagEqual = 4

    Flag = 5
    Variable = 6

    String = 7
    
    Word = 9

class Escape(TokenEnum):
    NoEscape = 0
    Start = 13
    Hex = 14
    Hex2 = 15

    @staticmethod
    def from_sub(sub: int) -> "Escape":
        return Escape(12 + sub if sub else 0)
