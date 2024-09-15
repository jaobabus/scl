import typing
from dataclasses import dataclass
from injector import inject


@dataclass
class ARDUnit:
    description: str = None
    scope: int = 0
    address: int = None
    
    @staticmethod
    def render_jump(unit: "ARDUnit") -> str:
        if unit.address is None:
            raise RuntimeError(f"Undefined reference to '{unit}'")
        addr = eval(f"ord({unit.address})") if isinstance(unit.address, str) else unit.address
        if addr > 0xFF:
            raise RuntimeError(f"Too long jump ({addr} > 0xFF)")
        sa = f"{unit.address}" if isinstance(unit.address, str) else f"0x{unit.address:02X}"
        return f"/* -- */ {sa}, // {unit.description}"
    
    def expand(self) -> list["ARDUnit"]:
        return [self]
    
    def setup(self, address: int) -> int:
        self.address = address
        return address

    def render(self) -> str:
        return ""

    def bytes_len(self) -> int: # for assert only
        return 0


@dataclass
class ARDPair(ARDUnit):
    key: int | str = None
    value: int | str = None
    unit_after: ARDUnit = None
    state_str: str = None
    
    def setup(self, address: int) -> int:
        assert self.key is not None
        assert self.value is not None
        self.address = address
        return address + self.bytes_len()
    
    def address_line(self, line: str, place: str = None) -> str:
        line = line.strip()
        return f"/* {self.address:02X} */ {line}" if place is None else f"/* {place} */ {line}"

    def render(self) -> str:
        hia = f" | ARD_HIA" if self.unit_after is not None else ''
        key = f"0x{self.key:02X}{hia}," if isinstance(self.key, int) else f"{self.key}{hia},"
        desc = f"// {self.description}" if self.description else ""
        value = f"0x{self.value:02X}, {desc}" if isinstance(self.value, int) else f"{self.value}, {desc}"
        out = [
            self.address_line(key),
            self.address_line(value, '--'),
        ]
        if self.unit_after is not None:
            out.append(ARDUnit.render_jump(self.unit_after))
        return '\n'.join(out)

    def bytes_len(self) -> int:
        return 3 if self.unit_after is not None else 2


class ARDConst(ARDUnit):
    def __init__(self, value: int, description: str = "const", **kw):
        super().__init__(address=value, description=description, **kw)


class Offset(ARDUnit):
    def __init__(self, off: int):
        super().__init__('')
        self.offset = off


class RulesList(ARDUnit, list[ARDUnit]):
    def __init__(self, rules: typing.Callable[[], list[ARDUnit]] | list[ARDUnit], **kw):
        super().__init__(**kw)
        self.rules = rules
    
    def expand(self) -> list[ARDUnit]:
        out = []
        for r in self:
            out.extend(r.expand())
        return out
        
    def setup(self, address: int) -> int:
        self.address = address
        if callable(self.rules):
            self.extend(self.rules())
        else:
            self.extend(self.rules)
        for i in self:
            address = i.setup(address)
        return address

    def render(self) -> str:
        _out = [f"/* {self.address:0>2X} */ // {self.description}"]
        for r in self:
            _out.extend(f"  {l}" for l in r.render().split('\n'))
        return '\n'.join(_out)

    def bytes_len(self) -> int:
        return sum(i.bytes_len() for i in self)


class RootRule(RulesList):
    def __init__(self, name: str, rules: list[ARDUnit], description: str = None):
        super().__init__(rules=rules, description=description)
        self.name = name


def render_rules(root: RootRule) -> str:
    a, l = root.setup(0), root.bytes_len()
    assert a == l
    _out = [f'SCL_STATIC_API_BEFORE_TYPE\nconst uint8_t {root.name}[] = {{']
    for rule in root:
        _out.append('\n'.join(map(lambda x: '  ' * rule.scope + x, rule.render().split('\n'))))
    _out.append('};')
    return '\n'.join(_out)


def inject_rule_table(path: str, table_id: str, table: str) -> int:
    return inject('ARD-Table', 'ARD-Table', path, table_id, table)
