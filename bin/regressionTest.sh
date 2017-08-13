mkdir -p regressionResult

ballRadius=0

if (( $# >= 1 ))
then
    ballRadius=$1
fi

serIDRegression="regressionTest"_$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

for((i=0;i<14;i++))
do
	echo Now $i
	date
	./executeAll.sh $ballRadius 1 500 $i 0.001
	./calGroovyBenchmark.sh $regressionResult
done


for i in 5 6 7 8
do
	echo Now $i
	date
	./executeAll.sh $ballRadius 1 500 $i 0.0
	./calGroovyBenchmark.sh $regressionResult
done




