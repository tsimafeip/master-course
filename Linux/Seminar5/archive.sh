ungunzip () {
	echo $1
	if [[ $1 == *.gz ]]
	then
		gunzip $1
		ungunzip ${1%%.gz}
	else
		cat $1
	fi
}

fn () {
	echo $1

	if [[ $1 == *.tar.gz ]]
	then
		for OUTPUT in $(tar -xvf $1 | grep '\.gz$')
		do
			fn $OUTPUT
		done
	else
		ungunzip $1
	fi
}

fn $1