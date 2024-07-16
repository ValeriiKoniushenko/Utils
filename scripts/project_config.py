import string_utils

gValidationErrors = []

# ================== CONFIGS ========================

gCharSet = "utf-8"

gIsIgnoreRootFolderValidation = True

gCheckFolderNameRule = string_utils.IsPascalCase

# from root dir
gTargetFolderValidationPath = [
    'sources',
    'tests',
    'benchmark'
]

# ====================================================