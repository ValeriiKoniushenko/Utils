import re

def IsPascalCase(s):
    return re.search('^([A-Z][a-z0-9]+)+$', s) != None
    

def IsCamelCase(s):
    return re.search('^[a-z]([a-z]*)([A-Z][a-z]*)+', s) != None