#!/bin/bash

# Paths and labels
promprt="$USER$ "
expc="expc_output"
mini="mini_output"
outfile="test.diff"

# Pulisci i file di output
> $expc
> $mini

#########################
# 1. Output da Bash
#########################
<testlist bash >$expc 2>&1

echo "BASH EXECUTED"

# Normalizza errori bash -> testlist
sed -i 's/^testlist: line [0-9]*: /testlist: /g' $expc
sed -i 's/^testlist: -c: line [0-9]*: /testlist: /g' $expc
sed -i 's/line [0-9]*: //g' $expc
sed -i 's/^bash: /testlist: /g' $expc

#########################
# 2. Output da minishell
#########################
<testlist ../minishell >$mini 2>&1

echo "MINISHELL EXECUTED"

# Rimuove prompt, clear screen e righe extra
sed -i "s/^$promprt//g" $mini             # Rimuove prompt
sed -i 's/^minishell% //' $mini           # Prompt alternativo
sed -i '/^exit$/d' $mini                  # Rimuove righe "exit"
sed -i '/^minishell% exit$/d' $mini       # Altra forma di exit
sed -i 's/minishell: /testlist: /g' $mini

#########################
# 3. Diff e risultato
#########################
diff $expc $mini > traces.diff

if [ "$?" = "1" ]; then
	echo KO
else
	echo OK
fi
