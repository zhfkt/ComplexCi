if (( $# >= 1 ))
then
    regressionResult=$1
else
    echo './calGroovyBenchmark.sh <regressionResult>'
    exit;
fi


logFile=`ls -t | head -1`
tail $logFile
fullLogFile=`readlink -f $logFile`
serId=`echo $logFile | sed -e 's/\.log$//'`

cd ../Master_algorithm

groovy  -cp 'target/algorithm-1.0-SNAPSHOT.jar:/root/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar'  src/main/java/org/dc/algorithm/NetMaster.groovy  ../data/networks/results/$serId/$serId.csv  ../data/networks.zip > /tmp/groovyResult
cat /tmp/groovyResult


valueResult=" score , "
timeResult=" time , "
minPointResult=" minPoint , "
for i in model1 model2 model3 model4 real1 real2 real3 real4
do
	valueResult=$valueResult`egrep $i /tmp/groovyResult  | awk -F ":" '{print $2}'`" , "
	timeResult=$timeResult`egrep "$i duration" $fullLogFile | awk -F ":" '{print $2}'`" , "
	minPointResult=$minPointResult`egrep "$i Min Point count" $fullLogFile | awk -F ":" '{print $2}'`" , "
done
valueResult=$valueResult`egrep "^[0-9]\.([0-9]*)" /tmp/groovyResult`
timeResult=$timeResult`egrep "duration" $fullLogFile | tail -1 | awk -F ":" '{print $2}'`
echo " $serId , model1 , model2 , model3 , model4 , real1 , real2 , real3 , real4 , total " >> $regressionResult
echo $valueResult   >> $regressionResult
echo $timeResult   >> $regressionResult
echo $minPointResult   >> $regressionResult

echo $regressionResult
cat $regressionResult
grep "score" $regressionResult | awk '{print $10}'
	
cd -