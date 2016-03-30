reference_sha1=$(git ls-tree origin/dev modules/sas-client | tr '\t' ' ' | cut -d ' ' -f 3)
GIT_DIR=.git/modules/modules/sas-client/ git fetch -p
master_sha1=$(GIT_DIR=.git/modules/modules/sas-client/ git rev-parse origin/clearwater2)
if [[ $reference_sha1 != $master_sha1 ]];
then
  echo "sas-client reference is out of date"
  echo "Marking build unstable"
fi
