serIDRegression="experimentResult"_$(date "+%y_%m_%d_%H_%M_%S")
regressionResult=`readlink -f regressionResult/$serIDRegression.csv`
echo $regressionResult

date

if false;
then

    for i in 7 8
    do
        echo Now $i

        ./executeAll.sh 2 1 500 $i 0.001
        ./calGroovyBenchmark.sh $regressionResult

    done

	for i in 11 12 13 14
	do
		echo Now $i

		./executeAll.sh 2 1 500 $i 0.001
		./calGroovyBenchmark.sh $regressionResult

	done
	
for j in 0 1 2
do	
	for i in 1 9
	do
		./executeAll.sh $j 1 500 $i 0.01
		./calGroovyBenchmark.sh $regressionResult
	done
done	

for j in  0.001 0.0 0.0001 0.01 0.1 
do
	./bestResult.sh 0 1 $j
	./calGroovyBenchmark.sh $regressionResult
done

	
fi


for j in 0 1 2
do	
	./executeAll.sh $j 1 500 10 0.01
	./calGroovyBenchmark.sh $regressionResult
done

date
