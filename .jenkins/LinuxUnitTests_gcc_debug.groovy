pipeline {
    agent { label 'Linux' }

    environment {
        C_COMPILER = 'gcc'
        CPP_COMPILER = 'g++'
        BUILD_TYPE = 'Debug'
    }

    stages {
        stage('Run') {
            steps {
                script {
                    copyArtifacts(
                        projectName: 'LinuxBuild_GCC_Debug',
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
