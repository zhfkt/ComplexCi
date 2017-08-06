for i in 5 7 8 1 13
do
	echo Now $i
	for j in 0 1 2
	do
		
		ratio=0.001
		
		if((i==13))
		then
			ratio=0.01
		fi
	
		./executeAll.sh $j 1 500 $i $ratio
		#sleep 5s
		logFile=`ls -t | head -1`
		tail $logFile
		serId=`echo $logFile | sed -e 's/\.log$//'`
		cd ../Master_algorithm
		groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip
		cd -
	done
done

./bestResult.sh
./quickResult.sh