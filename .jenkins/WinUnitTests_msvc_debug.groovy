pipeline {
    agent { label 'Windows' }

    environment {
        BUILD_TYPE = 'Release'
    }

    stages {
        stage('Run') {
            steps {
                script {
                    copyArtifacts(
                        projectName: 'WinBuild_MSVC_Release',
                        selector: lastCompleted(),
                        fingerprintArtifacts: true
                    )
                    def ARCHIVE_NAME = "${BUILD_TYPE}-Win64.zip"

                    bat """
                        dir
                        powershell Expand-Archive -Path ${ARCHIVE_NAME} -DestinationPath . -Force
                        build\\bin\\%BUILD_TYPE%\\UtilsTests.exe --gtest_output=xml:build\\bin\\%BUILD_TYPE%\\gtest_result.xml
                    """
                }
            }
        }
    }
}
