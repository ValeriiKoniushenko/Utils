pipeline {
    agent none

    triggers {
        cron('H 3 * * *')
    }

    stages {
        stage('Linux builds') {
            agent { label 'Linux' }

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
                    stage('Configure & Build') {
                        steps {
                            script {
                                def (C_COMPILER, CPP_COMPILER) = COMPILER_PAIR.split(':')

                                def buildDir = "build/${C_COMPILER}/${BUILD_TYPE}"
                                def attempt = 0
                                def maxAttempts = 2
                                def success = false

                                while (!success && attempt < maxAttempts) {
                                    attempt++

                                    if (attempt == 2) {
                                        echo "Previous build was FAILED. Let's try clear rebuild"
                                    }
                                    try {
                                        sh """
                                            # rm -rf build

                                            cmake -S . -B ${buildDir} \
                                                  -DCMAKE_C_COMPILER=${C_COMPILER}          \
                                                  -DCMAKE_CXX_COMPILER=${CPP_COMPILER}      \
                                                  -DCMAKE_BUILD_TYPE=${BUILD_TYPE}          \
                                                  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

                                            cmake --build ${buildDir} -- -j2
                                        """
                                    } catch(err) {
                                        echo "Build failed on attempt #${attempt}"
                                        if (attempt == maxAttempts) {
                                            error "Build failed after ${maxAttempts} attempts"
                                        }
                                    }
                                }
                            }
                        }
                    }

                    stage('Run') {
                        steps {
                            script {
                                def (C_COMPILER, CPP_COMPILER) = COMPILER_PAIR.split(':')

                                sh """
                                    build/${C_COMPILER}/${BUILD_TYPE}/tests/UtilsTests
                                """
                            }
                        }
                    }

                    stage('Package Artifacts') {
                        when {
                           expression { BUILD_TYPE == 'Release' }
                        }

                        steps {
                            script {
                                def (C_COMPILER, CPP_COMPILER) = COMPILER_PAIR.split(':')
                                def BUILD_PATH = "build/${C_COMPILER}/${BUILD_TYPE}"
                                def ARCHIVE_NAME = "${env.JOB_NAME}-${C_COMPILER}.tar.gz"

                                sh """
                                    # Remove old archive if it exists
                                    rm -f ${ARCHIVE_NAME}

                                    # Create tar.gz from install folder
                                    tar czf ${ARCHIVE_NAME} -C ${BUILD_PATH}/tests .
                                """
                                archiveArtifacts artifacts: "${ARCHIVE_NAME}", fingerprint: true
                            }
                        }
                    }
                }
            }
        }

        stage('Windows builds') {
            agent { label 'Windows' }

            matrix {
                axes {
                    axis {
                        name 'BUILD_TYPE'
                        values 'Debug', 'Release'
                    }
                }

                stages {
                    stage('Configure & Build') {
                        steps {
                            bat """
                                IF EXIST build rmdir /S /Q build

                                cmake -S . -B build -G "Visual Studio 17 2022" -A x64

                                cmake --build build --config %BUILD_TYPE% -- /m:2
                            """
                        }
                    }

                    stage('Run') {
                        steps {
                            bat """
                                build\\tests\\%BUILD_TYPE%\\UtilsTests.exe
                            """
                        }
                    }

                    stage('Package Artifacts') {
                        when {
                            expression { BUILD_TYPE == 'Release' }
                        }
                        steps {
                            script {
                                def BUILD_PATH = "build"
                                def ARCHIVE_NAME = "${env.JOB_NAME}-Win64.zip"

                                bat """
                                    if exist ${ARCHIVE_NAME} del /Q ${ARCHIVE_NAME}
                                    powershell Compress-Archive -Path ${BUILD_PATH}\\tests\\%BUILD_TYPE%\\* -DestinationPath ${ARCHIVE_NAME}
                                """
                                archiveArtifacts artifacts: "${ARCHIVE_NAME}", fingerprint: true
                            }
                        }
                    }
                }
            }
        }
    }
}
