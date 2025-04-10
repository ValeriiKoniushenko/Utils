import os
import subprocess
from scripts.prints import *


def CheckCommand(command, url):
    try:
        subprocess.run([command, '--version'], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        printSuccess(f"'{command}' was found!")
    except (subprocess.CalledProcessError, FileNotFoundError):
        printWarning(f"{command} wasn't found. Install it ({url}) and try again.")
        return False
    return True


def IsGitSafeDirectory(path):
    try:
        # Get the list of global safe directories
        result = subprocess.run(
            ["git", "config", "--global", "--get-all", "safe.directory"],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Normalize the path for comparison
        normalized_path = os.path.abspath(path)

        # Check if the directory is in the list
        safe_directories = result.stdout.splitlines()
        return normalized_path in safe_directories

    except subprocess.CalledProcessError:
        printError("Failed to retrieve Git safe directories.")
        return False

def CheckCommands():
    if CheckCommand("cmake", "https://cmake.org/download/") == False:
        return False

    if CheckCommand("git", "https://git-scm.com/downloads") == False:
        return False
    
    return True

def ProcessGitSafeDir():
    if IsGitSafeDirectory(os.getcwd()) == False:
        while True:
            user_input = input("This project wasn't added to safe dir inside your global Git config. Do thath automatically? [Y/n]: ").strip().lower()
            if user_input in ["y", "yes", ""]:
                git_config_command = ["git", "config", "--global", "--add", "safe.directory", os.getcwd()]
                try:
                    subprocess.run(git_config_command, check=True)
                except (subprocess.CalledProcessError, FileNotFoundError):
                    printError("Was met some errors while executing of git command.")
                    return False
                break
            elif user_input in ["n", "no"]:
                print("The project wasn't add to git safe dirs")
                break
            else:
                print("Invalid input. Please enter Y or N.")
    else:
        printSuccess("The project is inside git safe directory.")
    
    return True

def ProcessGitSubmodules():
    dependency_folder = "dependencies"
    count = 0
    try:
        dependencies = os.listdir(dependency_folder)
        count = len(dependencies)
        if (count != 0):
            for root, dirs, files in os.walk(dependency_folder):
                for dir in dirs:
                    count = len(dir)
                    if count != 0:
                        break

                if count != 0:
                        break
                        
                    
    except FileNotFoundError:
        printError("The folder '{dependency_folder}' does not exist.")
        return False



    if count == 0:
        while True:
            user_input = input("No dependencies found. Do you want to update git submodules? [Y/n]: ").strip().lower()
            if user_input in ["y", "yes", ""]:
                git_submodule_command = ["git", "submodule", "update", "--init", "--force", "--remote"]
                try:
                    subprocess.run(git_submodule_command, check=True)
                    print("Git submodules updated successfully.")
                except subprocess.CalledProcessError:
                    printError("Was met some error while trying to update git submodules.")
                    return False
                break
            elif user_input in ["n", "no"]:
                print("Skipping git submodule update. You can update manually using 'git submodule update --init --force --remote'.")
                break
            else:
                print("Invalid input. Please enter Y or N.")
    else:
        printSuccess("Git submodules don't need in update.")

    return True

CheckCommands()

if ProcessGitSafeDir() == False:
    exit(2)

if ProcessGitSubmodules() == False:
    exit(3)

frameMessage("SUCCESS INSTALL!")
