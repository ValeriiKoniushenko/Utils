import project_config
import project_utils
import sys
import os

if __name__ == "__main__":
    argv = sys.argv
    if len(argv) <= 1:
        project_config.gValidationErrors.append(f'[error] expected file name in the console arguments')
        exit(1)

    project_utils.SystemCheck()

    for i in range(1, len(argv)):
        path = argv[i]
        if not os.path.isabs(argv[i]):
            path = project_utils.GetRoot() + '\\' + argv[i]
            
        isCorrectFile = project_utils.ValidateFile(path)
        isCorrectFolder = project_utils.ValidateSubfolders(path)

        for error in project_config.gValidationErrors:
            print(error)

        if not isCorrectFile or not isCorrectFolder:
            exit(1)
    exit(0)
