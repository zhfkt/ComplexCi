@echo off

set argC=0
for %%x in (%*) do Set /A argC+=1


if %argC% LSS 3  (

	echo.
	echo traditionalCollectiveInfluence.cmd [networkPath] [ballRadius] [isPrintMinPointCausingMinComponent]
	echo.
	echo [networkPath] is the file path
	echo [ballRadius] is the Radius parameter defined in the Collective Influence Algorithm
	echo [isPrintMinPointCausingMinComponent] whether output limited point leading to 0.01 of giant component ratio or all points
	echo.
	echo Example: traditionalCollectiveInfluence.cmd \home\network\model1.csv 2 1
	
    goto :eof
)

set networkPath=%1
set ballRadius=%2
set isPrintMinPointCausingMinComponent=%3


..\..\x64\Release\ComplexCi %networkPath% %ballRadius% 1 500 1 0.01 %isPrintMinPointCausingMinComponent%
