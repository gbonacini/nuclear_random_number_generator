#!/usr/bin/env bash

which ent > /dev/null || { echo "error ent not installed " ; exit 1; }

[[ -f ./rnd_nums.txt ]] || { echo "missing input file" ; exit 1; }
cat rnd_nums.txt | grep -v '^CPM' | tr -d '\r\n' | head -204800 > rnd_nums.clean.txt

./convToBin.py rnd_nums.clean.txt rnd_nums.bin
[[ -f ./rnd_nums.bin ]] || { echo "error creating bin file " ; exit 1; }

ent -c rnd_nums.bin > rnd_nums.ent.bytes.report.txt
[[ -f ./rnd_nums.ent.bytes.report.txt ]] || { echo "error creating ent byte report file " ; exit 1; }
ent -b -c rnd_nums.bin > rnd_nums.ent.bites.report.txt
[[ -f ./rnd_nums.ent.bites.report.txt ]] || { echo "error creating ent byte report file " ; exit 1; }

[[ -f ./urandom.bin ]] || { echo "missing urandom output" ; exit 1; }
ent -c urandom.bin > urandom.ent.bytes.report.txt
[[ -f ./urandom.ent.bytes.report.txt ]] || { echo "error creating ent byte report file (urandom)" ; exit 1; }
ent -b -c urandom.bin > urandom.ent.bites.report.txt
[[ -f ./urandom.ent.bites.report.txt ]] || { echo "error creating ent byte report file (urandom)" ; exit 1; }

exit 0
