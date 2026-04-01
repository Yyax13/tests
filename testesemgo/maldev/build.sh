go build -ldflags="-s -w" -o $1 ./payload/src
mkdir -p ./build
mv $1 ./build/$1
echo "Successfuly compiled to $1, check it in build/$1"
