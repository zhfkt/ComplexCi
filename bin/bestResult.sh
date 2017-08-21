if (( $# >= 1 ))
then
    multiCoreMethod=$1
else
    multiCoreMethod=0
fi

if (( $# >= 2 ))
then
    batchSize=$2
else
    batchSize=1
fi

if (( $# >= 3 ))
then
    reinsertRatio=$3
else
    reinsertRatio=0.001
fi


serID=bestResult_$(date "+%y_%m_%d_%H_%M_%S")_multiCoreMethod_"$multiCoreMethod"_batchSize_"$batchSize"_reinsertRatio_"$reinsertRatio"
echo "serID: " $serID
exec > $serID.log

date


if (( $multiCoreMethod == 0 ))
then

./ComplexCi ../data/networks/model1.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/model2.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/model3.csv 2 $batchSize 500 15 $reinsertRatio &
./ComplexCi ../data/networks/model4.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real1.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real2.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real3.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real4.csv 2 $batchSize 500 18 $reinsertRatio &

elif (( $multiCoreMethod == 1 ))
then

./ComplexCi ../data/networks/model1.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/model2.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/model3.csv 2 $batchSize 500 11 $reinsertRatio &
./ComplexCi ../data/networks/model4.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real1.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real2.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real3.csv 2 $batchSize 500 14 $reinsertRatio &
./ComplexCi ../data/networks/real4.csv 2 $batchSize 500 14 $reinsertRatio &


elif (( $multiCoreMethod == 2 ))
then

./ComplexCi ../data/networks/model1.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/model2.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/model3.csv 2 $batchSize 500 15 $reinsertRatio &
./ComplexCi ../data/networks/model4.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real1.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real2.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real3.csv 2 $batchSize 500 18 $reinsertRatio &
./ComplexCi ../data/networks/real4.csv 2 $batchSize 500 18 $reinsertRatio &

else

exit

fi 



wait

resultFolder=../data/networks/results/$serID/ 

mkdir -p  $resultFolder
mv ../data/networks/*.csv_out  $resultFolder
cd $resultFolder

../../../../bin/mergeResult.sh 

date

cd -

