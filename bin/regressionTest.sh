for((i=0;i<19;i++))
do
	echo Now $i
	./executeAll.sh 0 1 500 $i 0.001
	#sleep 5s
	logFile=`ls -t | head -1`
	tail $logFile
	serId=`echo $logFile | sed -e 's/\.log$//'`
	cd ../Master_algorithm
	groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip
	cd -
done


