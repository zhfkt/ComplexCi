serID=bestResult_$(date "+%y_%m_%d_%H_%M_%S")_2_1_500_5_7_8_0.001
echo "serID: " $serID
exec > $serID.log

date

./ComplexCi ../data/networks/model1.csv 2 1 500 7 0.001  &
./ComplexCi ../data/networks/model2.csv 2 1 500 7 0.001  &
./ComplexCi ../data/networks/model3.csv 2 1 500 5 0.001  &
./ComplexCi ../data/networks/model4.csv 2 1 500 7 0.001  &
./ComplexCi ../data/networks/real1.csv 2 1 500 8 0.001  &
./ComplexCi ../data/networks/real2.csv 2 1 500 7 0.001  &
./ComplexCi ../data/networks/real3.csv 2 1 500 8 0.001  &
./ComplexCi ../data/networks/real4.csv 2 1 500 7 0.001  &

wait

resultFolder=../data/networks/results/$serID/ 

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh 

date


cd -

logFile=`ls -t | head -1`
tail $logFile
serId=`echo $logFile | sed -e 's/\.log$//'`
cd ../Master_algorithm
groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip