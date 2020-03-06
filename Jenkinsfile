pipeline {
  agent any
  stages {
    stage('build') {
      steps {
        echo 'this is a message'
        sh '''rm -f build
cmake build
cd build
make'''
      }
    }

    stage('test') {
      steps {
        echo 'test message'
      }
    }

    stage('static analysis') {
      steps {
        echo 'static analysis message'
      }
    }

  }
}