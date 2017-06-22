date

for i in `ls ../data/networks/*.csv`
do
   echo $i
   date
    ./ComplexCi $i 2 2500 500 &
   date
done

wait


date
