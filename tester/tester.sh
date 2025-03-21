#this tester expects to find an executable file "../minishell"
#!/bin/bash

promprt="minishell% "
expc="expected_output"
mini="mini_output"
outfile="test.diff"

#create expected output
bash testlist > $expc 2>&1
sed -i 's/^testlist: line [0-9]\+: /testlist: /g' $expc

< testlist ../minishell > $mini 2>&1
sed -i "s!^$promprt!!" $mini
sed -i 's\^minishell: \testlist: \g' $mini

diff expected_output mini_output > traces.diff

if [ "$?" = "1" ]; then
	echo KO;
else
	echo OK;
fi