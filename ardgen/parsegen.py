# Parser map Generator

from ardgen import *
from enums.parse import *


class DataAction(Enum):
    NoStore = 0
    Store8 = 1
    Store3 = 2
    Store4 = 3
    
    
NoStore = DataAction.NoStore
Store8 = DataAction.Store8
Store3 = DataAction.Store3
Store4 = DataAction.Store4


class State(int):
    def __init__(self, value = 0, switch = False):
        self._switch = switch
    
    @property
    def name(self) -> str:
        return f"SCL_PRIVATE_State({self})"

    @property
    def value(self) -> int:
        return int(self)

    @property
    def switch(self) -> bool:
        return self._switch


class ParseValue:
    def __init__(self, data: DataAction = DataAction.NoStore,
                 value: State | Token | None = None,
                 ret: bool = False):
        self.data = data
        self.ret = ret
        self.value = value

    def cpp(self) -> tuple[str, str]:
        desc = self.data.name
        base = f"SCL_PRIVATE_{self.data.name}"
        if isinstance(self.value, State):
            desc += f", state={self.value}"
            base += f" | SCL_PRIVATE_State({self.value}, {str(self.value.switch).lower()})"
        if isinstance(self.value, TokenEnum):
            desc += f", token={self.value.name}"
            base += f" | SCL_PRIVATE_Token({self.value.shlt_name()})"
        if self.ret:
            desc += f", return"
            base += f" | SCL_PRIVATE_Return"
        return base, desc

    def __str__(self) -> str: return ', // '.join(self.cpp())


class ParseRulePair(ARDPair):
    def __init__(self, ch: int | str, value: ParseValue, put: int | str = None):
        super().__init__(key=ch, value=value.cpp()[0], description='', unit_after=ARDConst(put) if put is not None else None)


escapes = [
    Escape.Start[0](lambda: [
        ParseRulePair("'x'", ParseValue(NoStore, Escape.Hex)),
        ParseRulePair("'r'", ParseValue(NoStore, ret=True), put=r"'\r'"),
        ParseRulePair("'b'", ParseValue(NoStore, ret=True), put=r"'\b'"),
        ParseRulePair("'e'", ParseValue(NoStore, ret=True), put=r"'\033'"),
        ParseRulePair("'a'", ParseValue(NoStore, ret=True), put=r"'\a'"),
        ParseRulePair("'v'", ParseValue(NoStore, ret=True), put=r"'\v'"),
        ParseRulePair("'n'", ParseValue(NoStore, ret=True), put=r"'\n'"),
        ParseRulePair(0, ParseValue(Store8, ret=True)),
    ]),
    Escape.Hex[0](lambda: [
        ParseRulePair(0, ParseValue(Store4, Escape.Hex2)),
    ]),
    Escape.Hex2[0](lambda: [
        ParseRulePair(0, ParseValue(Store4, ret=True)),
    ]),
]

asciis = [
    Token.Word[0](lambda: [
        ParseRulePair(r"'\"'", ParseValue(NoStore, Token.String)),
        ParseRulePair(r"'\''", ParseValue(NoStore, Token.String)),
        ParseRulePair(r"' '", ParseValue(NoStore, Token.Whitespace)),
        ParseRulePair(r"'\\'", ParseValue(NoStore, State(1), ret=True)),
        ParseRulePair(0, ParseValue(Store8, ret=True))
    ]),
    Token.Word[1](lambda: [
        ParseRulePair(0, ParseValue(Store8, State(0), ret=True))
    ]),
    Token.Flag[0](lambda: [
        ParseRulePair(r"'-'", ParseValue(NoStore, State(3), ret=True)),
        ParseRulePair(0, ParseValue(NoStore, State(3), ret=True))
    ]),
    Token.Flag[3](lambda: [
        ParseRulePair(r"'='", ParseValue(Store8, Token.FlagEqual, ret=True)),
        ParseRulePair(0, ParseValue(Store8, ret=True)) # /[0-9_a-zA-Z]
    ]),
    Token.FlagEqual[0](lambda: [
        ParseRulePair(0, ParseValue(NoStore, Token.NotInited))
    ]),
    Token.Variable[0](lambda: [
        ParseRulePair(0, ParseValue(NoStore, State(3), ret=True))
    ]),
    Token.Variable[3](lambda: [
        ParseRulePair(0, ParseValue(Store8, ret=True)) # /[0-9_a-zA-Z]
    ]),
]

strings = [
    Token.String[0](lambda: [
        ParseRulePair(r"'\"'", ParseValue(NoStore, State(1), ret=True)),
        ParseRulePair(r"'\''", ParseValue(NoStore, State(2), ret=True)),
    ]),
    Token.String[1](lambda: [
        ParseRulePair(r"'\\'", ParseValue(NoStore, Escape.Start, ret=True)),
        ParseRulePair(r"'\"'", ParseValue(NoStore, State(3), ret=True)),
        ParseRulePair(0, ParseValue(Store8, ret=True))
    ]),
    Token.String[2](lambda: [
        ParseRulePair(r"'\\'", ParseValue(NoStore, Escape.Start, ret=True)),
        ParseRulePair(r"'\''", ParseValue(NoStore, State(3), ret=True)),
        ParseRulePair(0, ParseValue(Store8, ret=True))
    ]),
    Token.String[3](lambda: [
        ParseRulePair(0, ParseValue(NoStore, Token.NotInited))
    ]),
]

specials = [
    Token.NotInited[0](lambda: [
        ParseRulePair(r"'$'", ParseValue(NoStore, Token.Variable)),
        ParseRulePair(r"'-'", ParseValue(NoStore, Token.Flag)),
        ParseRulePair(0, ParseValue(NoStore, Token.Word))
    ]),
    Token.Whitespace[0](lambda: [
        ParseRulePair(r"' '", ParseValue(Store8, ret=True)),
        ParseRulePair(r"'\t'", ParseValue(Store8, ret=True)),
        ParseRulePair(0, ParseValue(NoStore, Token.NotInited))
    ]),
    Token.Eof[0](lambda: [
        ParseRulePair(0, ParseValue(NoStore, Token.Eof, ret=True), put=0)
    ])
]


def make_token_id_map() -> tuple[str, str, str, str]:
    def extend(_rules: list[ARDUnit]) -> list[ARDUnit]:
        out = []
        for r in _rules:
            out.append(r)
            if isinstance(r, list):
                out.extend(extend(r))
        return out

    def jump_name(link: TokenPairLink) -> str:
        return f"{link.token.shlt_name()}[{link.state}]"
        
    tokens = {}
    for rule in extend(rules):
        if isinstance(rule, TokenPair):
            tokens[jump_name(rule)] = rule
    
    # 128 = len([token: bit[4]] [state: bit[2]]) -> 0x3F+1
    min_size = min(len(tokens) * 2 + 2, 128)
    
    _out = {}
    for k, v in tokens.items():
        _out[v.address] = _out.get(v.address, [])
        _out[v.address].append(k)
    for k, v in _out.items():
        v = '/'.join(v)
        _out[k] = f"    case 0x{k:02X}: return \"{v}\";"
    _out[-1] = f"    default: sprintf(buf, \"Unknown 0x%02hhX\", id); return buf;";
    ids = "    static char buf[1024];\n    switch(id) {{\n{}\n    }}".format('\n'.join(_out.values()))
    
    if min_size < 128:
        _out = []
        for k, v in sorted(tokens.items(), key=lambda x: x[1].token.value * 4 + x[1].state):
            v: TokenPairLink
            _out.append(f"    {{(0x40 | {v.token.shlt_name()} << 2) | {v.state}, 0x{v.address:02X}}},")
        _out.append(f"    {{0, 0x{rules[-1].address:02X}}}")
        return 'ARDMiniRule', '\n'.join(_out), f"    return find_rule((const uint8_t*)token_id_map, 0x40 | state).value;", ids
    else:
        _out = [f"    {rules[0].address},"] * 128
        for k, v in sorted(tokens.items(), key=lambda x: x[1].token.value * 4 + x[1].state):
            _out[k.token.value * 4 + k.token.state](f"    {v.address},")
        return 'uint8_t', '\n'.join(_out), f"    return token_id_map[state];", ids


rules = RootRule('rules', [
    *specials,
    *asciis,
    *strings,
    *escapes,
    Token.StateError[0](lambda: [ ParseRulePair(0, ParseValue(ret=True), put=1) ])
])


str_rules = render_rules(rules)

id_map = make_token_id_map()

content = f"""

/* Rules mapping
 * 
 * Token:
 *   [escape: bit[2]] [token: bit[4]] [state: bit[2]]
 *
 * Key:
 *   [char: bit[7]]
 *
 * Value:
 *   [data: bit[2]] [ret: bit] [0: bit] [0: bit] [0: bit[3]]
 *   [data: bit[2]] [ret: bit] [0: bit] [1: bit] [0: bit] [state: bit[2]]
 *   [data: bit[2]] [ret: bit] [1: bit] [token: bit[4]]
 *
 */
#define SCL_PRIVATE_DataAction(a) ((a) << 6)
#define SCL_PRIVATE_NoStore SCL_PRIVATE_DataAction(0)
#define SCL_PRIVATE_Store8  SCL_PRIVATE_DataAction(1)
#define SCL_PRIVATE_Store3  SCL_PRIVATE_DataAction(2)
#define SCL_PRIVATE_Store4  SCL_PRIVATE_DataAction(3)
#define SCL_PRIVATE_Return  0x20
#define SCL_PRIVATE_Token(token) (0x10 | (token))
#define SCL_PRIVATE_State(state, switch) (0x08 | (state) | ((switch) ? 0x4 : 0x0))

SCL_STATIC_API_BEFORE_TYPE
const {id_map[0]} token_id_map[] = {{
{id_map[1]}
}};

SCL_STATIC_API_BEFORE_TYPE
uint8_t get_id_by_token_state(uint8_t state)
{{
    if (state & 0xC0)
        state = (SHLT_EscapeStart - 1 + (state >> 6)) << 2;
{id_map[2]}
}}

SCL_STATIC_API_BEFORE_TYPE
uint8_t get_id_by_token(uint8_t token, uint8_t state)
{{
    return get_id_by_token_state((token << 2) | state);
}}

SCL_STATIC_API_BEFORE_TYPE
const char* dbg_name_of_rulemap(uint8_t id)
{{
{id_map[3]}
}}


{str_rules}
""".strip()

import os
print(os.path.abspath(os.curdir))

print("Success"
      if inject_rule_table("../src/parse.c", "ParseRules", content)
      else "Error")
