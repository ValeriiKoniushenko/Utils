pipeline {
    agent { label 'Linux' }

    environment {
        C_COMPILER = 'clang'
        CPP_COMPILER = 'clang++'
        BUILD_TYPE = 'Release'
    }

    stages {
        stage('Run') {
            steps {
                script {
                    copyArtifacts(
                        projectName: 'LinuxBuild_Clang_Release',
                        selector: lastCompleted(),
                        fingerprintArtifacts: true
                    )
                    sh """
                        tar -xzvf ${C_COMPILER}-${BUILD_TYPE}.tar.gz

                        cd build/${C_COMPILER}/${BUILD_TYPE}/bin
                        ./UtilsTests --gtest_output=xml:gtest_result.xml
                        cd ../../../../
                    """
                }
            }
        }

        stage('Gen Test Results') {
            steps {
                script {
                    junit "build/${C_COMPILER}/${BUILD_TYPE}/bin/gtest_result.xml"
                }
            }
        }
    }
}
