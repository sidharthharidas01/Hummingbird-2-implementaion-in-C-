# Hummingbird-2-implementaion-in-C++
Implementation oh Hummingbird-2 Cryptography algorithm in C++

This implementation is based on the following works: 
1. Engels, D., Saarinen, MJ.O., Schweitzer, P., Smith, E.M. (2012). The Hummingbird-2 Lightweight Authenticated Encryption Algorithm. In: Juels, A., Paar, C. (eds) RFID. Security and Privacy. RFIDSec 2011. Lecture Notes in Computer Science, vol 7055. Springer, Berlin, Heidelberg. https://doi.org/10.1007/978-3-642-25286-0_2
2. Zhang, Kai, Lin Ding and Jie Guan. “Cryptanalysis of Hummingbird-2.” IACR Cryptol. ePrint Arch. 2012 (2012): 207.

##Initializing
64-bit Initialization vector IV ={IV1, IV2, IV, IV4} and 128 bit Secret Key K = {K1, K2, K3, K4, K5, K6, 7, K8} is given as input. A 128-bit internal state register R is initialized.

##Encryption and Decryption
Encryption and decryption is done on 16 bit blocks of data. After each encryption/decryption state R has to be updated

Encryption is implemented in encrypt.cpp. Takes 128 bit key as an 8 element array of uint16_t type, 64 bit IV as an 4 element array of uint16_t type, plain text data as array of 16 bit blocks. It outputs cipher text as 16 bit blocks.

Decryption is implemented in decrypt.cpp. Takes 128 bit key as an 8 element array of uint16_t type, 64 bit IV as an 4 element array of uint16_t type, cipher text data as array of 16 bit blocks. It outputs plain text as 16 bit blocks.

Note that both keys and IV should be equal for encrypting and decrypting data.
