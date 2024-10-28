import subprocess
import shutil
import sys
import os
from packaging import version
from pathlib import Path

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def GetRoot():
    for path in Path(__file__).parents:
        git_dir = path / ".git"
        if git_dir.is_dir():
            return os.path.normpath(path)

def СheckForSetuptools():
    checkPipModuleCommand = subprocess.Popen(['pip', 'list'], stdout=subprocess.PIPE)
    result = str(checkPipModuleCommand.communicate()[0])

    if not 'setuptools' in result:
        eprint('[error]\t\t "setuptools" (python\'s pip module) wasn\'t found')
    else:
        print('[successful]\t "setuptools" was found!')


def CheckForCppCheck():
    if shutil.which('cppcheck') == None:
        eprint('[error]\t\t "cppcheck" wasn\'t found. Try to install it corresponding to your OS')
    else:
        print('[successful]\t "cppcheck" was found!')


def CheckEnvVar(name, messege=None):
    result = str(os.environ[name])
    if result == '':
        if (messege != None):
            eprint(messege)
        else:
            eprint(f"[error]\t\t Wasn't found environment varialbe: {name} - set it and try again")
    else:
        print(f'[successful]\t Environment variable "{name}" was found!')


def CheckCmakeVersion(reqVersion):
    output = subprocess.check_output(['cmake', '--version']).decode('utf-8')
    line = output.splitlines()[0]
    versionStr = line.split()[2]
    
    if version.parse(versionStr) < version.parse(reqVersion):
        eprint(f"[warning]\t Required cmake version is {reqVersion} but your cmake is: {versionStr}")
    else:
        print(f'[successful]\t Your cmake version({versionStr}) corresponding to the project\'s requirements')

def CheckForClang(reqVersion):
    if shutil.which('clang') == None:
        return
    
    print(f"[info]\t\t clang was found")

    output = subprocess.check_output(['clang', '--version']).decode('utf-8')
    line = output.splitlines()[0]
    versionStr = line.split()[2]
    
    if version.parse(versionStr) < version.parse(reqVersion):
        eprint(f"[warning]\t If you will use clang to compile a project, so: required clang version is {reqVersion} but your clang is: {versionStr}")
    else:
        print(f'[successful]\t Your clang version({versionStr}) corresponding to the project\'s requirements')


def CheckForGitCRLF():
    localSettings = subprocess.check_output(['git', 'config', '--list', '--local']).decode('utf-8')
    if 'core.autocrlf' in localSettings:
        if 'core.autocrlf=false' in localSettings:
            eprint(f"[warning]\t git core.autocrlf locally set to 'false'. Better to change it to 'true'")
        else:
            print(f"[successful]\t git core.autocrlf set to 'true'")
    else:
        settings = subprocess.check_output(['git', 'config', '--list', '--local']).decode('utf-8')
        if 'core.autocrlf=false' in settings:
            eprint(f"[warning]\t git core.autocrlf globally set to 'false'. Better to change it to 'true' globally or locally")
        else:
            print(f"[successful]\t git core.autocrlf set to 'true'")

def CheckForSubmodules():
    rootPath = GetRoot()
    if rootPath == None:
        eprint(f"[info]\t\t git root directory wasn't found")
        return

    anySubModuleName = "glm"
    dependenciesDirName = "dependencies"
    path = str(rootPath) + os.path.sep + dependenciesDirName + os.path.sep + anySubModuleName
    if not Path(path).exists():
        eprint('[error]\t\t Install all needed submodule using i.g. next command: git submodule update --recursive --remote')
    else:
        print(f"[successful]\t Dependencies was found")

def PrintAdditionalInfo(string):
    print(f"[info]\t\t {string}")


СheckForSetuptools()
CheckForCppCheck()
CheckCmakeVersion("3.30")
CheckForClang("18.1.8")
CheckForGitCRLF()
CheckForSubmodules()
CheckEnvVar("LD_LIBRARY_PATH")

print()
PrintAdditionalInfo("For better experience you can use alredy existing pre-commit hook. Just copy it from /scripts/git_hooks/pre-commit to /.git/hooks/")
