# Console Buffer Map Generator

from ardgen import *
from enums.cbe import *




class EscapeRule(ARDPair):
    def __init__(self, key: str | int, action: Action, unit_after: ARDUnit = None, ret: bool = True, **kw):
        options = " | SCL_PRIVATE_Return" if ret else ""
        super().__init__(key=key, value=f"{action.name}{options}", unit_after=unit_after, **kw)


skip_rules = RulesList(description="Skip rules", rules=[
    EscapeRule(r"'~'", Skip),
    EscapeRule(0, Fallback)
])

l_brace_escapes = RulesList(description="lbrace escapes", rules=[
    EscapeRule(r"'A'", CursorUp, description='Key arrow up'),
    EscapeRule(r"'B'", CursorDown, description='Key arrow down'),
    EscapeRule(r"'C'", CursorRight, description='Key arrow right'),
    EscapeRule(r"'D'", CursorLeft, description='Key arrow left'),
    EscapeRule(r"'3'", DeleteAfter, skip_rules, description='Key delete'),
    EscapeRule(0, Fallback)
])

second_lvl_escapes = RulesList(description="2lvl escapes", rules=[
    EscapeRule(r"'['", NoAction, l_brace_escapes),
    EscapeRule(0, Fallback)
])

root_escapes = RulesList(description="Root escapes", rules=[
    EscapeRule(r"'\e'", NoAction, second_lvl_escapes),
    EscapeRule(r"'\x7F'", DeleteBefore, description='Backspace'),
    EscapeRule(r"'\n'", Execute, description='Execute command'),
    EscapeRule(r"'\r'", Execute, description='Execute command'),
    EscapeRule(0, Insert)
])

rules = RootRule('escape_rules', [
    root_escapes,
    l_brace_escapes,
    second_lvl_escapes,
    skip_rules
])


escapes = '\n'.join(f"    {k} = {v.value}," for k, v in Action.__members__.items())

content = f"""

/*
 * EscapeRule:
 *   [0: bit[3]] [ret: bit] [action: bit[4]]
 *
 */

#define SCL_PRIVATE_Return 0x10

typedef enum {{ 
{escapes}
}} Action;

{render_rules(rules)}

""".strip()

print("Success"
      if inject_rule_table("../src/consolebuffer.c", "EscapeTable", content)
      else "Error")
