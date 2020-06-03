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

assert 85 "5+20*4;"
assert 72 "a = 3*4;b = 6;a*b;"
assert 19 "a = 4;b = 9;a+b+6;"
assert 10 "count = 3;2*count+4;"
assert 14 "count = 10;plus = 4;count+plus;"
assert 4 "return 4;4+6;return 6;"
assert 3 "count = 1;if (count) return 3;else return 4;"
assert 4 "count = 0;if (count) return 3;else return 4;"
assert 4 "if (0) a = 1;else a = 0;if (a) return 3;else return 4;"
echo OK