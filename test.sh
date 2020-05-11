#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" == "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 85 "5+20*4"
assert 41 " 12 + 34 - 5  "
assert 31 "2*3 + (2 + 3)*5"
assert 11 "2*3 + 3/2*5"
assert 10 "-4+14"
assert 18 "-3*(-1-5)"
assert 1 "1 != 2"
assert 1 "1 <= 2"
assert 1 "1 == 1"
assert 0 "-1 >= 2"
assert 0 "1 < 1"
assert 1 "30 > 2*(3+4)"
assert 1 "1+3 == 2+2"
assert 0 "-3*4+4 >= (2+2)*4/10"
assert 1 "-3*4 == -2*-2*-3"
echo OK