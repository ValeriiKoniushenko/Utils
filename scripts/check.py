import subprocess
import shutil
import sys
import os
from packaging import version
from pathlib import Path

errors = 0

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def GetRoot():
    for path in Path(__file__).parents:
        git_dir = path / ".git"
        if git_dir.is_dir():
            return os.path.normpath(path)

def СheckForSetuptools():
    global errors
    checkPipModuleCommand = subprocess.Popen(['pip', 'list'], stdout=subprocess.PIPE)
    result = str(checkPipModuleCommand.communicate()[0])

    if not 'setuptools' in result:
        errors += 1
        eprint('\x1b[6;30;31m[error]\x1b[0m\t\t "setuptools" (python\'s pip module) wasn\'t found. Install it: https://pypi.org/project/setuptools/')
    else:
        print('\x1b[6;30;32m[successful]\x1b[0m\t "setuptools" was found!')


def CheckForCppCheck():
    global errors
    if shutil.which('cppcheck') == None:
        errors += 1
        eprint('\x1b[6;30;31m[error]\x1b[0m\t\t "cppcheck" wasn\'t found. Try to install it corresponding to your OS')
    else:
        print('\x1b[6;30;32m[successful]\x1b[0m\t "cppcheck" was found!')


def CheckEnvVar(name, messege=None):
    global errors
    if os.environ.get(name) is None:
        if (messege != None):
            errors += 1
            eprint(messege)
        else:
            eprint(f"[info]\t\t Wasn't found environment varialbe: {name} - set it and try again")
    else:
        print(f'\x1b[6;30;32m[successful]\x1b[0m\t Environment variable "{name}" was found!')


def CheckCmakeVersion(reqVersion):
    output = subprocess.check_output(['cmake', '--version']).decode('utf-8')
    line = output.splitlines()[0]
    versionStr = line.split()[2]
    
    if version.parse(versionStr) < version.parse(reqVersion):
        eprint(f"\x1b[6;30;33m[warning]\x1b[0m\t Required cmake version is {reqVersion} but your cmake is: {versionStr}")
    else:
        print(f'\x1b[6;30;32m[successful]\x1b[0m\t Your cmake version({versionStr}) corresponding to the project\'s requirements')

def CheckForClang(reqVersion):
    if shutil.which('clang') == None:
        return
    
    print(f"[info]\t\t clang was found")

    output = subprocess.check_output(['clang', '--version']).decode('utf-8')
    line = output.splitlines()[0]
    versionStr = line.split()[2]
    
    if version.parse(versionStr) < version.parse(reqVersion):
        eprint(f"\x1b[6;30;33m[warning]\x1b[0m\t If you will use clang to compile a project, so: required clang version is {reqVersion} but your clang is: {versionStr}")
    else:
        print(f'\x1b[6;30;32m[successful]\x1b[0m\t Your clang version({versionStr}) corresponding to the project\'s requirements')


def CheckForGitCRLF():
    localSettings = subprocess.check_output(['git', 'config', '--list', '--local']).decode('utf-8')
    if 'core.autocrlf' in localSettings:
        if 'core.autocrlf=false' in localSettings:
            eprint(f"\x1b[6;30;33m[warning]\x1b[0m\t git core.autocrlf locally set to 'false'. Better to change it to 'true'")
        else:
            print(f"\x1b[6;30;32m[successful]\x1b[0m\t git core.autocrlf set to 'true'")
    else:
        settings = subprocess.check_output(['git', 'config', '--list', '--local']).decode('utf-8')
        if 'core.autocrlf=false' in settings:
            eprint(f"\x1b[6;30;33m[warning]\x1b[0m\t git core.autocrlf globally set to 'false'. Better to change it to 'true' globally or locally")
        else:
            print(f"\x1b[6;30;32m[successful]\x1b[0m\t git core.autocrlf set to 'true'")

def CheckForSubmodules():
    global errors
    rootPath = GetRoot()
    if rootPath == None:
        eprint(f"[info]\t\t git root directory wasn't found")
        return

    anySubModuleName = "glm"
    dependenciesDirName = "dependencies"
    path = str(rootPath) + os.path.sep + dependenciesDirName + os.path.sep + anySubModuleName
    if not Path(path).exists() or len(os.listdir(path)) == 0:
        errors += 1
        eprint('\x1b[6;30;31m[error]\x1b[0m\t\t Install all needed submodule using i.g. next command: git submodule update --init --recursive --remote')
    else:
        print(f"\x1b[6;30;32m[successful]\x1b[0m\t Dependencies was found")

def PrintAdditionalInfo(string):
    print(f"[info]\t\t {string}")


if __name__ == "__main__":
    СheckForSetuptools()
    CheckForCppCheck()
    CheckCmakeVersion("3.30")
    CheckForClang("18.1.8")
    CheckForGitCRLF()
    CheckForSubmodules()
    CheckEnvVar("LD_LIBRARY_PATH")

    print()
    PrintAdditionalInfo("For better experience you can use alredy existing pre-commit hook. Just copy it from /scripts/git_hooks/pre-commit to /.git/hooks/")
    exit(errors)