pipeline {
    agent none

    environment {
        REPORT_DIR = 'build_reports'
    }

    triggers {
        cron('H 3 * * *')
    }

    stages {
        stage('static Code Analysis') {
            agent any

            steps {
                script {
                    sh '''
                        mkdir -p ${REPORT_DIR}

                        cppcheck --enable=all \
                            --suppress=missingIncludeSystem \
                            --suppress=unusedFunction \
                            --suppress=missingInclude \
                            --suppress=unknownMacro \
                            --xml --xml-version=2 sources/ 2> ${REPORT_DIR}/cppcheck.xml
                    '''
                }
            }
        }

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

                agent { label 'Linux' }

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
                                        success = true
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
                                    build/${C_COMPILER}/${BUILD_TYPE}/bin/UtilsTests
                                """
                            }
                        }
                    }

                    stage('Test coverage') {
                        when {
                           expression { BUILD_TYPE == 'Debug' && COMPILER_PAIR == "clang:clang++" }
                        }

                        steps {
                            script {
                                def BIN_PATH = "build/clang/Debug/bin"

                                sh """
                                    pwd
                                    ls -lhAt
                                    ls -lhAt build
                                    ls -lhAt build/clang
                                    ls -lhAt build/clang/Debug
                                    ls -lhAt ${BIN_PATH}

                                    llvm-profdata merge -output=${BIN_PATH}/default.profdata ${BIN_PATH}/default.profraw

                                    llvm-cov show ./${BIN_PATH}/UtilsTests \
                                        -instr-profile=${BIN_PATH}/default.profdata \
                                        -format=html \
                                        -output-dir=coverage_report \
                                        --ignore-filename-regex="(build/.*)|(tests/.*)"
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
                                    rm -f ${ARCHIVE_NAME}
                                    tar czf ${ARCHIVE_NAME} -C ${BUILD_PATH}/bin .
                                """
                                archiveArtifacts artifacts: "${ARCHIVE_NAME}", fingerprint: true
                            }
                        }
                    }
                }
            }
        }

        stage('Windows builds') {
            matrix {
                axes {
                    axis {
                        name 'BUILD_TYPE'
                        values 'Debug', 'Release'
                    }
                }

                agent { label 'Windows' }

                stages {
                    stage('Prepare') {
                        steps {
                            script {
                                isTriggeredByCron = currentBuild.getBuildCauses('hudson.triggers.TimerTrigger$TimerTriggerCause')

                                if (isTriggeredByCron) {
                                    echo "Clean build preparation due to Cron task."
                                    sh """
                                        IF EXIST build rmdir /S /Q build
                                    """
                                }
                            }
                        }
                    }

                    stage('Configure & Build') {
                        steps {
                            script {
                                def attempt = 0
                                def maxAttempts = 2
                                def success = false

                                while (!success && attempt < maxAttempts) {
                                    attempt++

                                    if (attempt == 2) {
                                        echo "Previous build was FAILED. Let's try clear rebuild"
                                    }
                                    try {
                                        bat """
                                            cmake -S . -B build -G "Visual Studio 17 2022" -A x64

                                            cmake --build build --config %BUILD_TYPE% -- /m:2
                                        """
                                        success = true
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
                            bat """
                                build\\bin\\%BUILD_TYPE%\\UtilsTests.exe
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
                                    powershell Compress-Archive -Path ${BUILD_PATH}\\bin\\%BUILD_TYPE%\\* -DestinationPath ${ARCHIVE_NAME}
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
