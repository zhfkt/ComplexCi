serID=regressionTestForDailyUse_$(date "+%y_%m_%d_%H_%M_%S")
echo "serID: " $serID
exec > $serID.log

cd ../dailyUse/linux/

./traditionalCollectiveInfluence.sh ../../data/networks/model1.csv 0 0 &
./newReinsertCollectiveInfluence.sh ../../data/networks/model2.csv 0 1 0 &
./cppCollectiveInfluence.sh ../../data/networks/model3.csv 0 1 0 &

cd -

wait

resultFolder=../data/networks/results/$serID/

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

echo "serID: " $serID

../../../../bin/mergeResult.sh

date

cd -

./calGroovyBenchmark.sh /tmp/regressionTestForDailyUse.result

echo 'model1 0.212114535'
echo 'model2 a little worse than 0.174871586'
echo 'model3 0.348837118'

