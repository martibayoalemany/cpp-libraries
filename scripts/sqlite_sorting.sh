#!/usr/bin/env bash

if ! -e `which sqlite3` >/dev/null; then
	apt install sqlite3
fi

cd ../data

export DATABASE=random_numbers.db

if -e $DATABASE >/dev/null; then
   rm $DATABASE
fi

sqlite3 -line $DATABASE '.import random_numbers.txt numbers'
	

export HERE_DOC=<<-EOF
create TABLE input_file (numbers INTEGER)
.import random_numbers.txt numbers
.timer on
select * from input_file LIMIT 10
select * from input_file order by numbers ASC LIMIT 10
create index sorting on input_file(numbers)
select * from input_file order by numbers ASC LIMIT 10
EOF
