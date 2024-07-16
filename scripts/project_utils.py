from pathlib import Path
import glob
import pathlib
import subprocess
import project_config
from pathlib import Path, PurePath    
import os
import distutils

def GetFileExtensions():
    return [ 'cpp', 'c', 'cxx', 'h', 'hpp' ]

def IsCorrectFileExtension(file):
    tokens = str(file).split('.')
    if len(tokens) > 1:
        if tokens[1] in GetFileExtensions():
            return True
    return False
        
def GetRoot():
    for path in Path(__file__).parents:
        git_dir = path / ".git"
        if git_dir.is_dir():
            return os.path.normpath(path)
        
def GetGitIgnore():
    return Path(f'{GetRoot()}/.gitignore').read_text().splitlines()

def SystemCheck():
    if not CheckForCppcheck():
        print("[error] Can't find Cppcheck.exe. Try to download it and add to system PATH")
        exit(1)


def CanGoTo(path):
    item = pathlib.PurePath(path).name
    if item == '.git':
        return False

    for ignored in GetGitIgnore():
        ignors = [f'{v.strip('\\')}' for v in glob.glob(ignored)]
            
        if item in ignors:
            return False

    return True

def ValidateFile(path):
    if IsCorrectFileExtension(path):
        proc = subprocess.Popen(f'cppcheck -j 20 --language=c++ --std=c++20 -q --platform=win64 --template="[{{severity}}] {{file}}:{{line}}[{{column}}]:\n\tID: {{id}}\n\tMessage: {{message}}\n" --suppress=missingIncludeSystem --suppress=uninitdata --verbose --error-exitcode=1 {path}', stdout=subprocess.PIPE)
        output = proc.stdout.read().decode(project_config.gCharSet)
        if len(output) > 0:
            project_config.gValidationErrors.append({output})
            return False
    return True
    

def ValidateFolder(path):
    originalErrorsCount = len(project_config.gValidationErrors)
    errorsCount = originalErrorsCount

    path = Path(path)

    if not CanGoTo(path):
        return

    root = GetRoot()

    if root == str(path.parent) and project_config.gIsIgnoreRootFolderValidation:
        return True
    
    for target in project_config.gTargetFolderValidationPath:
        targetPathAsStr = str(os.path.normpath(f'{root}/{target}'))
        pathAsStr = str(path)
        if targetPathAsStr in pathAsStr:
            if not project_config.gCheckFolderNameRule(PurePath(path).name):
                errorsCount += 1
                project_config.gValidationErrors.append(f'[error] expected PascalCase folder name but was got: {path}')

    return originalErrorsCount == errorsCount

def CheckForCppcheck():
    return distutils.spawn.find_executable("cppcheck.exe") != None

def ValidateSubfolders(path):
    originalErrorsCount = len(project_config.gValidationErrors)
    errorsCount = originalErrorsCount

    path = str(os.path.normpath(path))
    root = str(GetRoot())

    offset = -1 if os.path.isfile(path) else 0

    path = path[len(root):].strip('\\')
    pathItems = path.split('\\')

    finalPath = root
    end = len(pathItems) + offset
    for i in range(0, end):
        finalPath += '\\' + str(pathItems[i])
        errorsCount += 0 if ValidateFolder(finalPath) else 1

    return originalErrorsCount == errorsCount
    