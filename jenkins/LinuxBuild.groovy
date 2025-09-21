pipeline {
    agent { label 'Linux' }

    stages {
        stage('Linux builds') {
            matrix {
                axes {
                    axis {
                        name 'COMPILER_PAIR'
                        values 'gcc:g++', 'clang:clang++'
                    }

                    axis {
                        name 'BUILD_TYPE'
                        values 'Debug', 'Release'
                    }
                }

                stages {
                    stage('Prepare') {
                        steps {
                            script {
                                isTriggeredByCron = currentBuild.getBuildCauses('hudson.triggers.TimerTrigger$TimerTriggerCause')
                                if (isTriggeredByCron) {
                                    echo "Clean build preparation due to Cron task."
                                    sh """
                                        rm -rf build
                                    """
                                }
                            }
                        }
                    }

                    stage('Configure & Build') {
                        steps {
                            script {

                                def (C_COMPILER, CPP_COMPILER) = COMPILER_PAIR.split(':')

                                addEmbeddableBadgeConfiguration(
                                    id: "linuxBuild_${C_COMPILER}_${BUILD_TYPE}",
                                    subject: "Linux | ${C_COMPILER} | ${BUILD_TYPE}",
                                    status: "building",
                                    color: "blue"
                                )

                                def buildDir = "build/${C_COMPILER}/${BUILD_TYPE}"
                                def attempt = 0
                                def maxAttempts = 2
                                def success = false

                                while (!success && attempt < maxAttempts) {
                                    attempt++

                                    if (attempt == 2) {
                                        echo "Previous build was FAILED. Let's try clear rebuild"
                                        sh """
                                        # rm -rf build
                                        """
                                    }
                                    try {
                                        sh """
                                            cmake -S . -B ${buildDir} \
                                                  -DCMAKE_C_COMPILER=${C_COMPILER}          \
                                                  -DCMAKE_CXX_COMPILER=${CPP_COMPILER}      \
                                                  -DCMAKE_BUILD_TYPE=${BUILD_TYPE}          \
                                                  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

                                            cmake --build ${buildDir} -- -j2
                                        """
                                        success = true
                                    } catch(err) {
                                        echo "Build failed on attempt #${attempt}"
                                        if (attempt == maxAttempts) {
                                            error "Build failed after ${maxAttempts} attempts"
                                        }
                                    } finally {
                                        addEmbeddableBadgeConfiguration(
                                            id: "linuxBuild_${C_COMPILER}_${BUILD_TYPE}",
                                            subject: "Linux | ${C_COMPILER} | ${BUILD_TYPE}",
                                            status: (success ? "success" : "failed"),
                                            color: (success ? "green" : "red")
                                        )
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
