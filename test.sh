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

echo OK