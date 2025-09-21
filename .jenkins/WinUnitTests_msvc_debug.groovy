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
                        UtilsTests.exe --gtest_output=xml:gtest_result.xml
                    """
                }
            }
        }
    }
}
