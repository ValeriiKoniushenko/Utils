import re

class BaseStringPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        str_ptr = self.val['_string']
        if str_ptr == 0:
            return '""'
        try:
            str_value = str_ptr.string()
        except gdb.error:
            str_value = "<invalid memory>"
        return f'"{str_value}"'

def PrettyBaseString(val):
    if str(val.type) == 'Core::StringAtom' or re.match("Core::BaseString\<.*\>", str(val.type)):
        return BaseStringPrinter(val)
    else:
        return None

gdb.pretty_printers.append(PrettyBaseString)