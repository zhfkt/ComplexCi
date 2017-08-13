mkdir -p regressionResult

ballRadius=0

if (( $# >= 1 ))
then
    ballRadius=$1
fi

serIDRegression=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo " , model1 , model2 , model3 , model4 , real1 , real2 , real3 , real4 , total " > $regressionResult


for((i=0;i<19;i++))
do
	echo Now $i
	date
	./executeAll.sh $ballRadius 1 500 $i 0.001
	./calGroovyBenchmark.sh $regressionResult
done


