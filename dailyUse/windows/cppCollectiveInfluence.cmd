@echo off

set argC=0
for %%x in (%*) do Set /A argC+=1


if %argC% LSS 4  (

	echo.
	echo cppCollectiveInfluence.cmd [networkPath] [ballRadius] [updateBatch] [isPrintMinPointCausingMinComponent]
	echo.
	echo [networkPath] is the file path
	echo [ballRadius] is the Radius parameter defined in the Collective Influence Algorithm
	echo [updateBatch] batch size of deleted points per updating Collective Influence value
	echo [isPrintMinPointCausingMinComponent] whether output limited point leading to 0 of giant component ratio or all points
	echo.
	echo Example: cppCollectiveInfluence.cmd \home\network\model1.csv 2 500 1
	
    goto :eof
)

set networkPath=%1
set ballRadius=%2
set updateBatch=%3
set isPrintMinPointCausingMinComponent=%4


..\..\x64\Release\ComplexCi %networkPath% %ballRadius% %updateBatch% 500 9 0.01 %isPrintMinPointCausingMinComponent%
