if (( $# >= 1 ))
then
    regressionResult=$1
else
    echo './calGroovyBenchmark.sh <regressionResult>'
    exit;
fi


logFile=`ls -t | head -1`
tail $logFile
serId=`echo $logFile | sed -e 's/\.log$//'`

cd ../Master_algorithm

groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip > /tmp/groovyResult
cat /tmp/groovyResult


valueResult="$serId , "
for i in model1 model2 model3 model4 real1 real2 real3 real4
do
	valueResult=$valueResult`egrep $i /tmp/groovyResult  | awk -F ":" '{print $2}'`" , "
done
valueResult=$valueResult`egrep "^[0-9]\.([0-9]*)" /tmp/groovyResult`
echo $valueResult   >> $regressionResult


echo $regressionResult
cat $regressionResult
	
cd -