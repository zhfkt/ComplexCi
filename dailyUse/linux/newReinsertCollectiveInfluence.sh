if (( $# >= 4 ))
then
	networkPath=$1
    ballRadius=$2
	updateBatch=$3
	isPrintMinPointCausingMinComponent=$4
else
    echo './newReinsertCollectiveInfluence.sh <networkPath> <ballRadius> <updateBatch> <isPrintMinPointCausingMinComponent>'
	echo '<networkPath> is the file path'
	echo '<ballRadius> is the Radius parameter defined in the Collective Influence Algorithm'
	echo '<updateBatch> batch size of deleted points per updating Collective Influence value'
	echo '<isPrintMinPointCausingMinComponent> whether output limited point leading to 0 of giant component ratio or all points'
	echo './newReinsertCollectiveInfluence.sh /home/network/model1.csv 2 500 1'
	
    exit;
fi


../../bin/ComplexCi $networkPath $ballRadius $updateBatch 500 10 0.01 $isPrintMinPointCausingMinComponent
