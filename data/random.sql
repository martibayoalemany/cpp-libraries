create TABLE input_file (numbers INTEGER)
.import random_numbers.txt numbers
.timer on
select * from input_file LIMIT 10
select * from input_file order by numbers ASC LIMIT 10
create index sorting on input_file(numbers)
select * from input_file order by numbers ASC LIMIT 10

