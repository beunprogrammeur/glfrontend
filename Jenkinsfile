pipeline {
  agent any
  stages {
    stage('build') {
      steps {
        echo 'this is a message'
        sh '''pwd
ls -a'''
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