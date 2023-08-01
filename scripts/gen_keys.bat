@echo off

set private_key=binja_ecdsa.pem
set public_key=binja_ecdsa.pub

REM Verify recognition of openssl
set command=openssl
where openssl >nul 2>&1
if %errorlevel% NEQ 0 (
    echo FATAL: OpenSSL is not recognized as an operable program. OpenSSL is required for key generation and can be installed here: https://wiki.openssl.org/index.php/Binaries
    echo Exiting...
    exit /b 1
)

REM Check existence of keys
if exist %private_key% (
    echo FATAL: key already exist
    echo Exiting...
    exit /b 1
)

if exist %public_key% (
    echo FATAL: key already exist
    echo Exiting...
    exit /b 1
)

REM Generate private key for EC256 jwt
openssl ecparam -name prime256v1 -genkey -noout -out %private_key%
attrib +h +r +s %private_key%

REM Extract public key from private key
openssl ec -in %private_key% -pubout -out %public_key%
attrib +r +s %public_key%

echo Keys generated successfully.