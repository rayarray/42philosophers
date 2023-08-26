#/bin/sh
#if [[ -f ./errorlog ]]; then rm ./errorlog; fi
TESTDATE="$(date)"
mkdir "$TESTDATE"
FILESIZE=0
let loops=0
while [ $FILESIZE -eq 0 ]
do
	$(./philo 198 195 60 60 > /dev/null 2> "./$TESTDATE/errorlog")
#	FILESIZE=$(stat -c%s "./errorlog")
	FILESIZE=$(stat -f%z "./$TESTDATE/errorlog")
	let loops++
	printf $FILESIZE
	let divten=loops%10
	if [ $divten -eq 0 ]; then 
		printf " $loops " 
		printf "$loops"  > "./$TESTDATE/loops"
		date > "./$TESTDATE/lastrun"
	fi
	let divten=loops%50
	if [[ $divten -eq 0 ]]; then printf "\n"; fi
done
echo
echo $loops

