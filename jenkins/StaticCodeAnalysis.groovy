pipeline {
    agent { label 'Linux' }

    environment {
        REPORT_DIR = 'build_reports'
    }

    stages {
        stage('Static Code Analysis') {
            steps {
                script {
                    sh '''
                        mkdir -p ${REPORT_DIR}

                        cppcheck --enable=all \
                            --suppress=missingIncludeSystem \
                            --suppress=unusedFunction \
                            --suppress=missingInclude \
                            --suppress=unknownMacro \
                            --suppress=identicalInnerCondition \
                            --suppress=knownConditionTrueFalse \
                            --suppress=noExplicitConstructor \
                            --xml --xml-version=2 sources/ 2> ${REPORT_DIR}/cppcheck.xml
                    '''
                }
            }
        }
    }
    post {
        always {
            recordIssues(
                enabledForFailure: true,
                tools: [
                    cppCheck(pattern: 'build_reports/cppcheck.xml')
                ]
            )
        }
    }
}
