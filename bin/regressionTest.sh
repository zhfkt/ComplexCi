mkdir -p regressionResult

ballRadius=0

if (( $# >= 1 ))
then
    ballRadius=$1
fi

serID=$(date "+%y_%m_%d_%H_%M_%S")_"$ballRadius"
regressionResult=`readlink -f regressionResult/$serID.log`
echo "Regression Test" > $regressionResult

for((i=0;i<19;i++))
do
	echo Now $i
	date
	./executeAll.sh $ballRadius 1 500 $i 0.001
	#sleep 5s
	logFile=`ls -t | head -1`
	tail $logFile
	serId=`echo $logFile | sed -e 's/\.log$//'`
	cd ../Master_algorithm
	groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip

	echo "$ballRadius_$i" >> $regressionResult
	
	groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip | grep 'Result' >> $regressionResult
	
	cd -
done


