serID=quickResult_$(date "+%y_%m_%d_%H_%M_%S")
echo "serID: " $serID
exec > $serID.log

date

csvFiles=../data/networks/*.csv

./ComplexCi ../data/networks/model1.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/model2.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/model3.csv 0 1 500 9 0.01  &
./ComplexCi ../data/networks/model4.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/real1.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/real2.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/real3.csv 0 1 500 10 0.01  &
./ComplexCi ../data/networks/real4.csv 0 1 500 10 0.01  &

wait

resultFolder=../data/networks/results/$serID/ 

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh

date

cd -