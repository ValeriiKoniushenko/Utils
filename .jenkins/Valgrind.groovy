pipeline {
    agent { label 'Linux' }

    environment {
        C_COMPILER = 'gcc'
        BUILD_TYPE = 'Debug'
    }

    stages {
        stage('Run Valgrind') {
            steps {
                script {
                    copyArtifacts(
                        projectName: 'LinuxBuild_GCC_Debug',
                        selector: lastCompleted(),
                        fingerprintArtifacts: true
                    )

                    sh '''
                    tar -xzvf ${C_COMPILER}-${BUILD_TYPE}.tar.gz

                    valgrind \
                      --tool=memcheck \
                      --leak-check=full \
                      --show-leak-kinds=all \
                      --track-origins=yes \
                      --error-exitcode=42 \
                      --xml=yes \
                      --xml-file=valgrind-report.xml \
                      build/${C_COMPILER}/${BUILD_TYPE}/bin/UtilsTests
                    '''
                }
            }

            post {
                success {
                    addEmbeddableBadgeConfiguration(
                        id: "linux_valgrind",
                        subject: "Valgrind",
                        status: "passed",
                        color: "green"
                    )
                }
                failure {
                    addEmbeddableBadgeConfiguration(
                        id: "linux_valgrind",
                        subject: "Valgrind",
                        status: "failed",
                        color: "red"
                    )
                }
            }
        }
    }
    post {
        always {
            recordIssues enabledForFailure: true, tool: valgrind(pattern: 'valgrind-report.xml')
        }
    }
}
