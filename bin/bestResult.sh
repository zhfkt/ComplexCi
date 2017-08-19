serID=bestResult_$(date "+%y_%m_%d_%H_%M_%S")
echo "serID: " $serID
exec > $serID.log

date

./ComplexCi ../data/networks/model1.csv 2 1 500 18 0.001  &
./ComplexCi ../data/networks/model2.csv 2 1 500 18 0.001  &
./ComplexCi ../data/networks/model3.csv 2 1 500 15 0.001  &
./ComplexCi ../data/networks/model4.csv 2 1 500 18 0.001  &
./ComplexCi ../data/networks/real1.csv 2 1 500 14 0.001  &
./ComplexCi ../data/networks/real2.csv 2 1 500 18 0.001  &
./ComplexCi ../data/networks/real3.csv 2 1 500 14 0.001  &
./ComplexCi ../data/networks/real4.csv 2 1 500 18 0.001  &

wait

resultFolder=../data/networks/results/$serID/ 

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh 

date

cd -

