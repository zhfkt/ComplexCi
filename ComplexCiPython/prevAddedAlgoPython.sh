SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`

if (( $# >= 3 ))
then
        methodCentrality=$1
        csvFilesFolder=$2
        pythonOut=$3
else
    echo './prevAddedAlgoPython.sh [0] [../data/networks/] [../data/networks/pythonResults/] '
    exit;
fi

serID="prevAdded_"$(date "+%y_%m_%d_%H_%M_%S")"_method_${methodCentrality}"



echo "serID: " ${SCRIPTPATH}/${serID}.log

exec > ${SCRIPTPATH}/${serID}.log

date

resultFolder=$pythonOut/$serID/
mkdir -p  $resultFolder

for i in `ls ${csvFilesFolder}/*.csv`
do
   echo $i
   date
		python3 -u ComplexCiPython.py $i $resultFolder -1 10000000 500 $methodCentrality
   date
done

wait


${SCRIPTPATH}/../bin/mergeResult.sh ${resultFolder}

cd ${SCRIPTPATH}/../Master_algorithm/

groovy -cp "target/algorithm-1.0-SNAPSHOT.jar:`readlink -f ~`/.m2/repository/log4j/log4j/1.2.17/log4j-1.2.17.jar" src/main/java/org/dc/algorithm/NetMaster.groovy  ${resultFolder}/${serID}.csv  ../data/networks.zip

date