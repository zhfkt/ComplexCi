if (( $# >= 5 ))
then
    ballRadius=$1
    batch=$2
    outputBatch=$3
	method=$4
	reInsertRatio=$5
else
    echo './experimentResult.sh 2 500 500 0 0.001'
    exit;
fi

serIDRegression=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"_"$batch"_"$outputBatch"_"$method"_"$reInsertRatio"
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

date

./executeAll.sh $ballRadius $batch $outputBatch $method $reInsertRatio
./calGroovyBenchmark.sh $regressionResult

date
