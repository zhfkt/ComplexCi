serID=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"_"$batch"_"$outputBatch"_"$method"_"$reInsertRatio"
echo "serID: " $serID
exec > $serID.log

date

csvFiles=../data/networks/*.csv

for i in `ls $csvFiles`
do
   echo $i
    ./ComplexCi  0 1 500 14 0.01  &
done

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