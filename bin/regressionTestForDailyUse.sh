serID=bestResult_$(date "+%y_%m_%d_%H_%M_%S")_multiCoreMethod_"$multiCoreMethod"_batchSize_"$batchSize"_reinsertRatio_"$reinsertRatio"
echo "serID: " $serID
exec > $serID.log

cd ../dailyUse/linux/

./traditionalCollectiveInfluence.sh ../../data/networks/model1.csv 2 0 &
./newReinsertCollectiveInfluence.sh ../../data/networks/model2.csv 0 1 0 &
./cppCollectiveInfluence.sh ../../data/networks/model3.csv 2 1 0 &

cd -

wait

resultFolder=../data/networks/results/$serID/ 

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh 

date

cd -

./calGroovyBenchmark.sh /tmp/regressionTestForDailyUse.result

echo 'model1 0.207893888'
echo 'model2 a little worse than 0.174871586'
echo 'model3 0.34461667'

