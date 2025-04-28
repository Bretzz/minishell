#!/bin/bash

# Paths and labels
promprt="minishell% "
expc="expc_output"
mini="mini_output"
outfile="test.diff"

# Pulisci i file di output
> $expc
> $mini

#########################
# 1. Output da Bash
#########################
while IFS= read -r line; do
    echo "$ $line" >> $expc
    bash -c "$line" >> $expc 2>&1
done < testlist

# Normalizza errori bash -> testlist
sed -i 's/^bash: /testlist: /g' $expc
sed -i 's/^bash: -c: line [0-9]*: /testlist: /g' $expc

#########################
# 2. Output da minishell
#########################
while IFS= read -r line; do
    echo "$ $line" >> $mini
    echo "$line" | ../minishell >> $mini 2>&1
done < testlist

# Rimuove prompt, clear screen e righe extra
sed -i 's/\[H\[J//g' $mini              # Rimuove clear screen
sed -i "s/^$promprt//g" $mini             # Rimuove prompt
sed -i 's/^minishell% //' $mini           # Prompt alternativo
sed -i '/^exit$/d' $mini                  # Rimuove righe "exit"
sed -i '/^minishell% exit$/d' $mini       # Altra forma di exit
sed -i 's/^minishell: /testlist: /g' $mini

#########################
# 3. Diff e risultato
#########################
diff $expc $mini > traces.diff

if [ "$?" = "1" ]; then
	echo KO
else
	echo OK
fi
