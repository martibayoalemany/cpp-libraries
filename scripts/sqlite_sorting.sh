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
	

