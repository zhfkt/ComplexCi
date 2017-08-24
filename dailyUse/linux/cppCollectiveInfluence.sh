if (( $# >= 4 ))
then
	networkPath=$1
    ballRadius=$2
	batchSize=$3
	whetherOutputLimitedNode=$4
else
    echo './cppCollectiveInfluence.sh <networkPath> <ballRadius> <batchSize> <whetherOutputLimitedNode>'
	echo '<ballRadius> is the Radius parameter defined in the Collective Influence Algorithm'
	echo '<batchSize> batch size of deleted points per updating Collective Influence value'
	echo '<whetherOutputLimitedNode> whether output limited point leading to 0.01 of giant component ratio or all points'
	echo './cppCollectiveInfluence.sh /home/network/model1.csv 2 500 1'
	
    exit;
fi


../../bin/ComplexCi $networkPath $ballRadius $batchSize 500 9 0.01 $whetherOutputLimitedNode
