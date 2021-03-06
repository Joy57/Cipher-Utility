This is a stand-alone cipher-utility that uses openssl crypto-library AES_256 and SHA1 to encrypt/decrypt any file.


Prerequisites
-------------

| Software    | Version     |
| ----------- | ----------- |
| `openssl`   | [`1.0.2o`](https://www.openssl.org/source/)|

Check to see if you already have `openssl` on your machine.
```
$ openssl version
```

If you already have openssl installed on your system. Please check the version by typing `openssl version`. This program will work with any version of openssl. 

If not, install using the direction from openssl's site or follow the direction below:

For Ubuntu:

    
    $ sudo apt-get update && apt-get install -y vim openssl libssl-dev
    
    
For Mac using [Homebrew](https://brew.sh/):

    $ brew update
    $ brew install openssl
    
If you get `error: 'openssl/evp.h' file not found` then you might need to do the following:

    $ cd /usr/local/include 
    $ ln -s ../opt/openssl/include/openssl .
    

**Note:** If you have a version above `1.0.2o`. And you want to encrypt/decrypt with `cipher-utility` program and then encrypt/decrypt using built-in openssl command, you will need to pass the `-md md5` flag. Because the latest version uses `sha1` as a message digest by default.


How to run the program
----------------------

First, clone this repo:

```
    $ git clone https://github.com/Joy57/Cipher-Utility.git
```

1. Run the `make` command. 
2. Run executable file `./cipher`.

**Note:** If you modify or make changes to the code please make sure to run `make clean` and then follow the above step to run the program again.

When you run the executable `cipher`, it will provide the following direction for usage.

```
Usage: ./cipher -[e|d] [-nosalt] -in infile -out outfile -p password
```
By defualt the progam is set to `salt` for security purpose. It is recommended to use `salt` for any type of encryption/decryption especially with a stream cipher. 

Example usage:
-----------------------------
**Encryption using salt** 
![salt encryption][usage1]

**Decryption using salt**
![salt decryption][usage2]

if you would like to make sure your file wasn't corrupted or changed due to encryption/decryption. Type the following command

sample.jpg is your original file and dec.jpg is your decrypted file.
```
$ diff sample.jpg dec.jpg
```
Output should be nothing in the case of successful encryption/decryption.

If you want to encrypt/decrypt using `-nosalt`. Then type the following:

**Encrypt**
```
$ ./cipher -e -in sample.jpg -out enc.jpg -p iLoveMagic
```
**Decrypt**
```
$ ./cipher -d -in enc.jpg -out dec.jpg -p iLoveMagic
```

Incase you forget to pass the -p flag for password. You will be prompted to enter password.

![forgot password][usage3]



[usage1]: images/usage1.png
[usage2]: images/usage2.png
[usage3]: images/usage3.png

License
-------
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)








