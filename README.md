RSA Message Encryption and Authentication with X-509
========
Dogukan Cagatay 2010 @ Izmir University of Economics

This program shows a simple structural implementation of RSA key generation, message encryption and message authentication. The programs are written in C. It uses math.h and GMP GNU Multiple Precision library for the mathematical calculations of RSA keys. It can create a key as much bits as you want and encrypt, sign, decrypt and  auhtenticate the message with that key.

There are three modules in the program:
- Key generator(create_rsa_keys)
- Message encryption and sign (send_message)
- Message decryption and authentication (authenticate_msg)

Full documentation is available in Doxygen format. You can open it by doc.html .
