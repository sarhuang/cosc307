if [ -d bin -o -d src -o -d include ]; then
  echo 'One of bin or src or include already exists -- you need to start from a fresh directory'
  exit 1
fi

mkdir bin
cp -r /home/jplank/cs302/Labs/Lab8/src .
cp -r /home/jplank/cs302/Labs/Lab8/include .
cp /home/jplank/cs302/Labs/Lab8/makefile .
