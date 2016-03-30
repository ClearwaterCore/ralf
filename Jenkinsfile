node {
  git url: 'git@github.com:Metaswitch/ralf.git', branch: 'rkd-experimental-jenkins-2.0'

  stage 'Check location'
  sh 'pwd'
  sh 'ls'

  stage 'Check git status'
  sh 'git status'

  stage 'Check cpp-common'
  sh './check-cpp-common.sh'

  stage 'Check sas-client'
  sh './check-sas-client.sh'

  stage 'Build'
  sh 'make'

  stage 'UT'
  sh 'make full_test'

  stage 'Package'
  sh 'make deb'
}
