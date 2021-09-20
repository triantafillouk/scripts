#/usr/bin/bash
echo ------ go version ------
time ./anonl > anonl_go.out

echo ------ c version ------
time ./anonl_c > anonl_c.out

echo ------ cc_stl version ------
time ./anonl_cc_stl > anonl_cc_stl.out

echo ------ cc_stl1 version ------
time ./anonl_cc_stl1 > anonl_cc_stl1.out

echo ------ perl version ------
time ./anonl.pl > anonl_pl.out
