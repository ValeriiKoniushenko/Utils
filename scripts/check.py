import subprocess
import shutil
import sys
import os
import glob
from packaging import version
from pathlib import Path

errors = 0
pathToRepo = None

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def GetRoot():
    global pathToRepo
    if pathToRepo != None:
        return os.path.normpath(pathToRepo)
    
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
        eprint('[error]\t\t "setuptools" (python\'s pip module) wasn\'t found. Install it: https://pypi.org/project/setuptools/')
    else:
        print('[successful]\t "setuptools" was found!')


def CheckForCppCheck():
    global errors
    if shutil.which('cppcheck') == None:
        errors += 1
        eprint('[error]\t\t "cppcheck" wasn\'t found. Try to install it corresponding to your OS')
    else:
        print('[successful]\t "cppcheck" was found!')


def CheckEnvVar(name, messege=None):
    global errors
    if os.environ.get(name) is None:
        if (messege != None):
            errors += 1
            eprint(messege)
        else:
            eprint(f"[info]\t\t Wasn't found environment varialbe: {name} - set it and try again")
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
    global errors
    rootPath = GetRoot()
    if rootPath == None:
        eprint(f"[info]\t\t git root directory wasn't found")
        return

    dependenciesDirName = "dependencies"
    path = str(rootPath) + os.path.sep + dependenciesDirName + os.path.sep

    hasError = True
    if Path(path).exists() and len(os.listdir(path)) != 0:
        for topPath in glob.iglob(path + '*', recursive=False):
            if len(os.listdir(topPath)) == 0:
                break

        hasError = False
        print(f"[successful]\t Dependencies was found")
    
    if hasError:
        errors += 1
        eprint('[error]\t\t Install all needed submodule using i.g. next command: git submodule update --init --recursive --remote')
    
def PrintAdditionalInfo(string):
    print(f"[info]\t\t {string}")

def ValidateArgs():
    global pathToRepo

    mainArg = sys.argv[1]

    if str(mainArg).strip() == '--help':
        print(f"Run {sys.argv[0]} with next params:")
        print(f"--root \t checking a project by the given path. But be attentive, this path must contains .git folder")
        print(f"--help \t to get help message")
        return True
    elif str(mainArg).strip() == '--root':
        if len(sys.argv) < 3:
            print(f"Incorrect count of arguments. Run with the next template: {sys.argv[0]} --root \"path/to/your/repo\"")
            return True
        
        pathToRepo = os.path.normpath(sys.argv[2])
        if not Path(pathToRepo).is_absolute():
            pathToRepo = Path(pathToRepo).resolve()
    else:
        print("Incorrect argument[s]")
        return True

    return False

if __name__ == "__main__":
    if len(sys.argv) >= 2:
        if ValidateArgs():
            exit(1)

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