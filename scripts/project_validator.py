import project_config
import project_utils
import os

def ValidateWholeProject(rootPath):
    rootPath = os.path.normpath(rootPath)

    if not project_utils.CanGoTo(rootPath):
        return

    directory = os.fsencode(rootPath)
    os.chdir(directory)

    for item in os.listdir(directory):
        path = os.path.normpath(f'{rootPath}/{item.decode(project_config.gCharSet)}')
        if os.path.isdir(path):
            project_utils.ValidateFolder(path)
            ValidateWholeProject(path)
            os.chdir(rootPath)
        else:
            project_utils.ValidateFile(path)

    return 

if __name__ == "__main__":
    project_utils.SystemCheck()

    ValidateWholeProject(project_utils.GetRoot())

    for error in project_config.gValidationErrors:
        print(error)

    exit(len(project_config.gValidationErrors) != 0)