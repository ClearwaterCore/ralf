reference_sha1=$(git ls-tree origin/dev modules/cpp-common | tr '\t' ' ' | cut -d ' ' -f 3)
GIT_DIR=.git/modules/modules/cpp-common/ git fetch -p
master_sha1=$(GIT_DIR=.git/modules/modules/cpp-common/ git rev-parse origin/master)
if [[ $reference_sha1 != $master_sha1 ]];
then
  echo "cpp-common reference is out of date"
  echo "Marking build unstable"
fi
