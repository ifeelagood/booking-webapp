#!/bin/bash

PRIVATE_KEY="binja_ecdsa.pem"
PUBLIC_KEY="binja_ecdsa.pub"

# check existence of openssl
if ! command -v openssl &> /dev/null
then
    echo "FATAL: openssl could not be found"
    exit
fi

# check existence of keys
if [ -f $PRIVATE_KEY ] || [ -f $PUBLIC_KEY ]; then
    echo "FATAL: key(s) already exist"
    exit
fi

# for EC256 jwt token
openssl ecparam -name prime256v1 -genkey -noout -out $PRIVATE_KEY
chmod 400 $PRIVATE_KEY

openssl ec -in $PRIVATE_KEY -pubout -out $PUBLIC_KEY
chmod 444 $PUBLIC_KEY

echo "Keys generated successfully"