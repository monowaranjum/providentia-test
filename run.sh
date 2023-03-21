cd Test
FILE_NAME=$1
FOLDER="Data_${FILE_NAME}"
mkdir -p $FOLDER
cd $FOLDER
rm -rf *
clang -S -emit-llvm -o ./target.ll ../$FILE_NAME
mkdir -p ProvenanceData
cd ProvenanceData
rm -rf *
cd ..
opt -load ../../build/musl-llvm-pass/libBlockExtractPass.so -basic-block-extract ./target.ll -enable-new-pm=0> /dev/null
cd ProvenanceData
python3 ../../pg_generator.py
