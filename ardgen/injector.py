
import regex


def inject(head_prefix: str, tail_prefix: str, path: str, table_id: str, table: str) -> int:
    pre = "// THIS BLOCK AUTOGENERATED\n// Do NOT change it"
    template = "// -- {head} '{id}' --\n{pre}\n// {{\n\n{body}\n\n// }}\n// -- {tail} --"
    inject_count = 0

    def repl(_: regex.Match) -> str:
        nonlocal inject_count
        inject_count += 1
        return template.format(id=table_id, body=table, pre=pre, head=head_prefix, tail=tail_prefix)

    file = open(path, 'r', encoding='utf-8').read()
    prologue = rf"(?<pro>^\/\/ -- {head_prefix} '{table_id}' --){{0}}"
    _inject = rf"(?<inj>^\/\/ -- Inject-here {head_prefix} '{table_id}' --){{0}}"
    epilogue = rf"(?<epi>^\/\/ -- {tail_prefix} --){{0}}"
    comments = r"(?<comm>^\/\/[^\n]*\n){0}"
    string = r"(?<str>(\'([^\\\']|\\.)*\')|(\"([^\\\"]|\\.)*\")){0}"
    body = r"(?<body>\{([^{}\"\']|(?&body)|(?&str))*\}){0}"
    rex = regex.compile(prologue + _inject + epilogue + string + body + comments +
                        rf"((?&pro)\s*(?&comm)*\s*// (?&body)\s*(?&epi))|(?&inj)",
                        flags=regex.M)
    res = rex.sub(repl, file)
    if res != file:
        with open(path, 'w', encoding='utf-8') as ofl:
            ofl.write(res)
    return inject_count
