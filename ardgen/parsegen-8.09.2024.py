# Parser map Generator

from ardgen import *
from etoken.enums import *


whitespace_map = RulesList("Whitespace", lambda: [
    CommonRule(' ', "store8"),
    CommonRule('\\t', "store8"),
    OtherRule("token8(SHLT_None)", none_map)
])

escape_parser_map = RulesList("Escape parser", [
    CommonRule('\\x01', "store4", Offset(+1)),
    CommonRule('\\x02', "store4 | next_mask", Offset(0)),
    CommonRule('\\x01', "store3", Offset(+1)),
    CommonRule('\\x02', "store3", Offset(+1)),
    CommonRule('\\x03', "store3 | next_mask", Offset(0)),
    OtherRule('0')
])

escape_tree_map = RulesList("Escape tree", [
    CommonRule('x', "0", escape_parser_map[0]),
    OtherRule("next_mask")
])

string_sq_map = RulesList("StringSQ", lambda: [
    CommonRule('\\\\', "0", escape_parser_map),
    CommonRule('"', "no_store | token8(SHLT_None) | next_mask", none_map),
    OtherRule("store8")
])

string_dq_map = RulesList("StringDQ", lambda: [
    CommonRule('\\\\', "0", escape_parser_map),
    CommonRule('\'', "no_store | token8(SHLT_None) | next_mask", none_map),
    OtherRule("store8")
])

string_sp_map = RulesList("StringSP", lambda: [
    CommonRule('\\\\', "0", escape_parser_map),
    CommonRule('`', "no_store | token8(SHLT_None) | next_mask", none_map),
    OtherRule("store8")
])

argument_map = RulesList("Argument", lambda: [
    CommonRule('`', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\'', "no_store | token8(SHLT_None)", none_map),
    CommonRule('"', "no_store | token8(SHLT_None)", none_map),
    CommonRule(' ', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\\t', "no_store | token8(SHLT_None)", none_map),
    OtherRule("store8 | next_mask")
])

flag_map = RulesList("Flag", lambda: [
    CommonRule('`', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\'', "no_store | token8(SHLT_None)", none_map),
    CommonRule('"', "no_store | token8(SHLT_None)", none_map),
    CommonRule(' ', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\\t', "no_store | token8(SHLT_None)", none_map),
    OtherRule("store8 | next_mask")
])

variable_map = RulesList("Variable", lambda: [
    CommonRule('`', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\'', "no_store | token8(SHLT_None)", none_map),
    CommonRule('"', "no_store | token8(SHLT_None)", none_map),
    CommonRule(' ', "no_store | token8(SHLT_None)", none_map),
    CommonRule('\\t', "no_store | token8(SHLT_None)", none_map),
    OtherRule("store8 | next_mask")
])

none_map = RulesList("None", [
    CommonRule(' ', "token8(SHLT_Whitespace)", whitespace_map),
    CommonRule('\\t', "token8(SHLT_Whitespace)", whitespace_map),
    CommonRule('"', "token8(SHLT_StringDQ) | next_mask", string_sq_map),
    CommonRule('\'', "token8(SHLT_StringSQ) | next_mask", string_dq_map),
    CommonRule('`', "token8(SHLT_SubProgram) | next_mask", string_sp_map),
    CommonRule('-', "token8(SHLT_Flag)", flag_map),
    CommonRule('$', "token8(SHLT_Variable)", variable_map),
    OtherRule("token8(SHLT_Argument)", argument_map)
])

rules = [
    CommonRule('0', '0'),
    none_map,
    whitespace_map,
    string_sq_map,
    string_dq_map,
    string_sp_map,
    argument_map,
    flag_map,
    variable_map,
    escape_tree_map,
    escape_parser_map
]


rules = render_rules(rules, 'rules')

content = f"""
static const uint8_t escape_tree_id = {escape_tree_map.id};
static const uint8_t escape_parser_id = {escape_parser_map.id};
static const ARDMiniMap token_id_map[] = {{
    {{{none_map.id} + 1, 0}},
    {{{none_map.id} + 1, 1}},
    {{{none_map.id} + 1, 2}},
    {{{whitespace_map.id} + 1, 3}},
    {{{none_map.id} + 1, 4}},
    {{{flag_map.id} + 1, 5}},
    {{{none_map.id} + 1, 6}},
    {{{variable_map.id} + 1, 7}},
    {{{string_sq_map.id} + 1, 8}},
    {{{string_dq_map.id} + 1, 9}},
    {{{string_sp_map.id} + 1, 10}},
    {{{argument_map.id} + 1, 11}},
    {{0}}
}};

{rules}
""".strip()

import os
print(os.path.abspath(os.curdir))

print("Success"
      if inject_rule_table("../src/parse.c", "ParseRules", content)
      else "Error")
