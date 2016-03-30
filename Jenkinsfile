node {
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
