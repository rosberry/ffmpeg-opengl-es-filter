aes-test$(EXEEXT): aes-test.$(OBJEXT)
	$(LINK) aes-test.$(OBJEXT) $(TEST_OBJS) -o aes-test$(EXEEXT)

arcfour-test$(EXEEXT): arcfour-test.$(OBJEXT)
	$(LINK) arcfour-test.$(OBJEXT) $(TEST_OBJS) -o arcfour-test$(EXEEXT)

arctwo-test$(EXEEXT): arctwo-test.$(OBJEXT)
	$(LINK) arctwo-test.$(OBJEXT) $(TEST_OBJS) -o arctwo-test$(EXEEXT)

blowfish-test$(EXEEXT): blowfish-test.$(OBJEXT)
	$(LINK) blowfish-test.$(OBJEXT) $(TEST_OBJS) -o blowfish-test$(EXEEXT)

cast128-test$(EXEEXT): cast128-test.$(OBJEXT)
	$(LINK) cast128-test.$(OBJEXT) $(TEST_OBJS) -o cast128-test$(EXEEXT)

base16-test$(EXEEXT): base16-test.$(OBJEXT)
	$(LINK) base16-test.$(OBJEXT) $(TEST_OBJS) -o base16-test$(EXEEXT)

base64-test$(EXEEXT): base64-test.$(OBJEXT)
	$(LINK) base64-test.$(OBJEXT) $(TEST_OBJS) -o base64-test$(EXEEXT)

camellia-test$(EXEEXT): camellia-test.$(OBJEXT)
	$(LINK) camellia-test.$(OBJEXT) $(TEST_OBJS) -o camellia-test$(EXEEXT)

chacha-test$(EXEEXT): chacha-test.$(OBJEXT)
	$(LINK) chacha-test.$(OBJEXT) $(TEST_OBJS) -o chacha-test$(EXEEXT)

cnd-memcpy-test$(EXEEXT): cnd-memcpy-test.$(OBJEXT)
	$(LINK) cnd-memcpy-test.$(OBJEXT) $(TEST_OBJS) -o cnd-memcpy-test$(EXEEXT)

des-test$(EXEEXT): des-test.$(OBJEXT)
	$(LINK) des-test.$(OBJEXT) $(TEST_OBJS) -o des-test$(EXEEXT)

des3-test$(EXEEXT): des3-test.$(OBJEXT)
	$(LINK) des3-test.$(OBJEXT) $(TEST_OBJS) -o des3-test$(EXEEXT)

md2-test$(EXEEXT): md2-test.$(OBJEXT)
	$(LINK) md2-test.$(OBJEXT) $(TEST_OBJS) -o md2-test$(EXEEXT)

md4-test$(EXEEXT): md4-test.$(OBJEXT)
	$(LINK) md4-test.$(OBJEXT) $(TEST_OBJS) -o md4-test$(EXEEXT)

md5-test$(EXEEXT): md5-test.$(OBJEXT)
	$(LINK) md5-test.$(OBJEXT) $(TEST_OBJS) -o md5-test$(EXEEXT)

md5-compat-test$(EXEEXT): md5-compat-test.$(OBJEXT)
	$(LINK) md5-compat-test.$(OBJEXT) $(TEST_OBJS) -o md5-compat-test$(EXEEXT)

memeql-test$(EXEEXT): memeql-test.$(OBJEXT)
	$(LINK) memeql-test.$(OBJEXT) $(TEST_OBJS) -o memeql-test$(EXEEXT)

memxor-test$(EXEEXT): memxor-test.$(OBJEXT)
	$(LINK) memxor-test.$(OBJEXT) $(TEST_OBJS) -o memxor-test$(EXEEXT)

gosthash94-test$(EXEEXT): gosthash94-test.$(OBJEXT)
	$(LINK) gosthash94-test.$(OBJEXT) $(TEST_OBJS) -o gosthash94-test$(EXEEXT)

ripemd160-test$(EXEEXT): ripemd160-test.$(OBJEXT)
	$(LINK) ripemd160-test.$(OBJEXT) $(TEST_OBJS) -o ripemd160-test$(EXEEXT)

hkdf-test$(EXEEXT): hkdf-test.$(OBJEXT)
	$(LINK) hkdf-test.$(OBJEXT) $(TEST_OBJS) -o hkdf-test$(EXEEXT)

salsa20-test$(EXEEXT): salsa20-test.$(OBJEXT)
	$(LINK) salsa20-test.$(OBJEXT) $(TEST_OBJS) -o salsa20-test$(EXEEXT)

sha1-test$(EXEEXT): sha1-test.$(OBJEXT)
	$(LINK) sha1-test.$(OBJEXT) $(TEST_OBJS) -o sha1-test$(EXEEXT)

sha224-test$(EXEEXT): sha224-test.$(OBJEXT)
	$(LINK) sha224-test.$(OBJEXT) $(TEST_OBJS) -o sha224-test$(EXEEXT)

sha256-test$(EXEEXT): sha256-test.$(OBJEXT)
	$(LINK) sha256-test.$(OBJEXT) $(TEST_OBJS) -o sha256-test$(EXEEXT)

sha384-test$(EXEEXT): sha384-test.$(OBJEXT)
	$(LINK) sha384-test.$(OBJEXT) $(TEST_OBJS) -o sha384-test$(EXEEXT)

sha512-test$(EXEEXT): sha512-test.$(OBJEXT)
	$(LINK) sha512-test.$(OBJEXT) $(TEST_OBJS) -o sha512-test$(EXEEXT)

sha512-224-test$(EXEEXT): sha512-224-test.$(OBJEXT)
	$(LINK) sha512-224-test.$(OBJEXT) $(TEST_OBJS) -o sha512-224-test$(EXEEXT)

sha512-256-test$(EXEEXT): sha512-256-test.$(OBJEXT)
	$(LINK) sha512-256-test.$(OBJEXT) $(TEST_OBJS) -o sha512-256-test$(EXEEXT)

sha3-permute-test$(EXEEXT): sha3-permute-test.$(OBJEXT)
	$(LINK) sha3-permute-test.$(OBJEXT) $(TEST_OBJS) -o sha3-permute-test$(EXEEXT)

sha3-224-test$(EXEEXT): sha3-224-test.$(OBJEXT)
	$(LINK) sha3-224-test.$(OBJEXT) $(TEST_OBJS) -o sha3-224-test$(EXEEXT)

sha3-256-test$(EXEEXT): sha3-256-test.$(OBJEXT)
	$(LINK) sha3-256-test.$(OBJEXT) $(TEST_OBJS) -o sha3-256-test$(EXEEXT)

sha3-384-test$(EXEEXT): sha3-384-test.$(OBJEXT)
	$(LINK) sha3-384-test.$(OBJEXT) $(TEST_OBJS) -o sha3-384-test$(EXEEXT)

sha3-512-test$(EXEEXT): sha3-512-test.$(OBJEXT)
	$(LINK) sha3-512-test.$(OBJEXT) $(TEST_OBJS) -o sha3-512-test$(EXEEXT)

serpent-test$(EXEEXT): serpent-test.$(OBJEXT)
	$(LINK) serpent-test.$(OBJEXT) $(TEST_OBJS) -o serpent-test$(EXEEXT)

twofish-test$(EXEEXT): twofish-test.$(OBJEXT)
	$(LINK) twofish-test.$(OBJEXT) $(TEST_OBJS) -o twofish-test$(EXEEXT)

version-test$(EXEEXT): version-test.$(OBJEXT)
	$(LINK) version-test.$(OBJEXT) $(TEST_OBJS) -o version-test$(EXEEXT)

knuth-lfib-test$(EXEEXT): knuth-lfib-test.$(OBJEXT)
	$(LINK) knuth-lfib-test.$(OBJEXT) $(TEST_OBJS) -o knuth-lfib-test$(EXEEXT)

cbc-test$(EXEEXT): cbc-test.$(OBJEXT)
	$(LINK) cbc-test.$(OBJEXT) $(TEST_OBJS) -o cbc-test$(EXEEXT)

cfb-test$(EXEEXT): cfb-test.$(OBJEXT)
	$(LINK) cfb-test.$(OBJEXT) $(TEST_OBJS) -o cfb-test$(EXEEXT)

ctr-test$(EXEEXT): ctr-test.$(OBJEXT)
	$(LINK) ctr-test.$(OBJEXT) $(TEST_OBJS) -o ctr-test$(EXEEXT)

gcm-test$(EXEEXT): gcm-test.$(OBJEXT)
	$(LINK) gcm-test.$(OBJEXT) $(TEST_OBJS) -o gcm-test$(EXEEXT)

eax-test$(EXEEXT): eax-test.$(OBJEXT)
	$(LINK) eax-test.$(OBJEXT) $(TEST_OBJS) -o eax-test$(EXEEXT)

ccm-test$(EXEEXT): ccm-test.$(OBJEXT)
	$(LINK) ccm-test.$(OBJEXT) $(TEST_OBJS) -o ccm-test$(EXEEXT)

cmac-test$(EXEEXT): cmac-test.$(OBJEXT)
	$(LINK) cmac-test.$(OBJEXT) $(TEST_OBJS) -o cmac-test$(EXEEXT)

poly1305-test$(EXEEXT): poly1305-test.$(OBJEXT)
	$(LINK) poly1305-test.$(OBJEXT) $(TEST_OBJS) -o poly1305-test$(EXEEXT)

chacha-poly1305-test$(EXEEXT): chacha-poly1305-test.$(OBJEXT)
	$(LINK) chacha-poly1305-test.$(OBJEXT) $(TEST_OBJS) -o chacha-poly1305-test$(EXEEXT)

hmac-test$(EXEEXT): hmac-test.$(OBJEXT)
	$(LINK) hmac-test.$(OBJEXT) $(TEST_OBJS) -o hmac-test$(EXEEXT)

umac-test$(EXEEXT): umac-test.$(OBJEXT)
	$(LINK) umac-test.$(OBJEXT) $(TEST_OBJS) -o umac-test$(EXEEXT)

meta-hash-test$(EXEEXT): meta-hash-test.$(OBJEXT)
	$(LINK) meta-hash-test.$(OBJEXT) $(TEST_OBJS) -o meta-hash-test$(EXEEXT)

meta-cipher-test$(EXEEXT): meta-cipher-test.$(OBJEXT)
	$(LINK) meta-cipher-test.$(OBJEXT) $(TEST_OBJS) -o meta-cipher-test$(EXEEXT)

meta-aead-test$(EXEEXT): meta-aead-test.$(OBJEXT)
	$(LINK) meta-aead-test.$(OBJEXT) $(TEST_OBJS) -o meta-aead-test$(EXEEXT)

meta-armor-test$(EXEEXT): meta-armor-test.$(OBJEXT)
	$(LINK) meta-armor-test.$(OBJEXT) $(TEST_OBJS) -o meta-armor-test$(EXEEXT)

buffer-test$(EXEEXT): buffer-test.$(OBJEXT)
	$(LINK) buffer-test.$(OBJEXT) $(TEST_OBJS) -o buffer-test$(EXEEXT)

yarrow-test$(EXEEXT): yarrow-test.$(OBJEXT)
	$(LINK) yarrow-test.$(OBJEXT) $(TEST_OBJS) -o yarrow-test$(EXEEXT)

xts-test$(EXEEXT): xts-test.$(OBJEXT)
	$(LINK) xts-test.$(OBJEXT) $(TEST_OBJS) -o xts-test$(EXEEXT)

pbkdf2-test$(EXEEXT): pbkdf2-test.$(OBJEXT)
	$(LINK) pbkdf2-test.$(OBJEXT) $(TEST_OBJS) -o pbkdf2-test$(EXEEXT)

sexp-test$(EXEEXT): sexp-test.$(OBJEXT)
	$(LINK) sexp-test.$(OBJEXT) $(TEST_OBJS) -o sexp-test$(EXEEXT)

sexp-format-test$(EXEEXT): sexp-format-test.$(OBJEXT)
	$(LINK) sexp-format-test.$(OBJEXT) $(TEST_OBJS) -o sexp-format-test$(EXEEXT)

rsa2sexp-test$(EXEEXT): rsa2sexp-test.$(OBJEXT)
	$(LINK) rsa2sexp-test.$(OBJEXT) $(TEST_OBJS) -o rsa2sexp-test$(EXEEXT)

sexp2rsa-test$(EXEEXT): sexp2rsa-test.$(OBJEXT)
	$(LINK) sexp2rsa-test.$(OBJEXT) $(TEST_OBJS) -o sexp2rsa-test$(EXEEXT)

bignum-test$(EXEEXT): bignum-test.$(OBJEXT)
	$(LINK) bignum-test.$(OBJEXT) $(TEST_OBJS) -o bignum-test$(EXEEXT)

random-prime-test$(EXEEXT): random-prime-test.$(OBJEXT)
	$(LINK) random-prime-test.$(OBJEXT) $(TEST_OBJS) -o random-prime-test$(EXEEXT)

pkcs1-test$(EXEEXT): pkcs1-test.$(OBJEXT)
	$(LINK) pkcs1-test.$(OBJEXT) $(TEST_OBJS) -o pkcs1-test$(EXEEXT)

pkcs1-sec-decrypt-test$(EXEEXT): pkcs1-sec-decrypt-test.$(OBJEXT)
	$(LINK) pkcs1-sec-decrypt-test.$(OBJEXT) $(TEST_OBJS) -o pkcs1-sec-decrypt-test$(EXEEXT)

pss-test$(EXEEXT): pss-test.$(OBJEXT)
	$(LINK) pss-test.$(OBJEXT) $(TEST_OBJS) -o pss-test$(EXEEXT)

rsa-sign-tr-test$(EXEEXT): rsa-sign-tr-test.$(OBJEXT)
	$(LINK) rsa-sign-tr-test.$(OBJEXT) $(TEST_OBJS) -o rsa-sign-tr-test$(EXEEXT)

pss-mgf1-test$(EXEEXT): pss-mgf1-test.$(OBJEXT)
	$(LINK) pss-mgf1-test.$(OBJEXT) $(TEST_OBJS) -o pss-mgf1-test$(EXEEXT)

rsa-pss-sign-tr-test$(EXEEXT): rsa-pss-sign-tr-test.$(OBJEXT)
	$(LINK) rsa-pss-sign-tr-test.$(OBJEXT) $(TEST_OBJS) -o rsa-pss-sign-tr-test$(EXEEXT)

rsa-test$(EXEEXT): rsa-test.$(OBJEXT)
	$(LINK) rsa-test.$(OBJEXT) $(TEST_OBJS) -o rsa-test$(EXEEXT)

rsa-encrypt-test$(EXEEXT): rsa-encrypt-test.$(OBJEXT)
	$(LINK) rsa-encrypt-test.$(OBJEXT) $(TEST_OBJS) -o rsa-encrypt-test$(EXEEXT)

rsa-keygen-test$(EXEEXT): rsa-keygen-test.$(OBJEXT)
	$(LINK) rsa-keygen-test.$(OBJEXT) $(TEST_OBJS) -o rsa-keygen-test$(EXEEXT)

rsa-sec-decrypt-test$(EXEEXT): rsa-sec-decrypt-test.$(OBJEXT)
	$(LINK) rsa-sec-decrypt-test.$(OBJEXT) $(TEST_OBJS) -o rsa-sec-decrypt-test$(EXEEXT)

rsa-compute-root-test$(EXEEXT): rsa-compute-root-test.$(OBJEXT)
	$(LINK) rsa-compute-root-test.$(OBJEXT) $(TEST_OBJS) -o rsa-compute-root-test$(EXEEXT)

dsa-test$(EXEEXT): dsa-test.$(OBJEXT)
	$(LINK) dsa-test.$(OBJEXT) $(TEST_OBJS) -o dsa-test$(EXEEXT)

dsa-keygen-test$(EXEEXT): dsa-keygen-test.$(OBJEXT)
	$(LINK) dsa-keygen-test.$(OBJEXT) $(TEST_OBJS) -o dsa-keygen-test$(EXEEXT)

curve25519-dh-test$(EXEEXT): curve25519-dh-test.$(OBJEXT)
	$(LINK) curve25519-dh-test.$(OBJEXT) $(TEST_OBJS) -o curve25519-dh-test$(EXEEXT)

ecc-mod-test$(EXEEXT): ecc-mod-test.$(OBJEXT)
	$(LINK) ecc-mod-test.$(OBJEXT) $(TEST_OBJS) -o ecc-mod-test$(EXEEXT)

ecc-modinv-test$(EXEEXT): ecc-modinv-test.$(OBJEXT)
	$(LINK) ecc-modinv-test.$(OBJEXT) $(TEST_OBJS) -o ecc-modinv-test$(EXEEXT)

ecc-redc-test$(EXEEXT): ecc-redc-test.$(OBJEXT)
	$(LINK) ecc-redc-test.$(OBJEXT) $(TEST_OBJS) -o ecc-redc-test$(EXEEXT)

ecc-sqrt-test$(EXEEXT): ecc-sqrt-test.$(OBJEXT)
	$(LINK) ecc-sqrt-test.$(OBJEXT) $(TEST_OBJS) -o ecc-sqrt-test$(EXEEXT)

ecc-dup-test$(EXEEXT): ecc-dup-test.$(OBJEXT)
	$(LINK) ecc-dup-test.$(OBJEXT) $(TEST_OBJS) -o ecc-dup-test$(EXEEXT)

ecc-add-test$(EXEEXT): ecc-add-test.$(OBJEXT)
	$(LINK) ecc-add-test.$(OBJEXT) $(TEST_OBJS) -o ecc-add-test$(EXEEXT)

ecc-mul-g-test$(EXEEXT): ecc-mul-g-test.$(OBJEXT)
	$(LINK) ecc-mul-g-test.$(OBJEXT) $(TEST_OBJS) -o ecc-mul-g-test$(EXEEXT)

ecc-mul-a-test$(EXEEXT): ecc-mul-a-test.$(OBJEXT)
	$(LINK) ecc-mul-a-test.$(OBJEXT) $(TEST_OBJS) -o ecc-mul-a-test$(EXEEXT)

ecdsa-sign-test$(EXEEXT): ecdsa-sign-test.$(OBJEXT)
	$(LINK) ecdsa-sign-test.$(OBJEXT) $(TEST_OBJS) -o ecdsa-sign-test$(EXEEXT)

ecdsa-verify-test$(EXEEXT): ecdsa-verify-test.$(OBJEXT)
	$(LINK) ecdsa-verify-test.$(OBJEXT) $(TEST_OBJS) -o ecdsa-verify-test$(EXEEXT)

ecdsa-keygen-test$(EXEEXT): ecdsa-keygen-test.$(OBJEXT)
	$(LINK) ecdsa-keygen-test.$(OBJEXT) $(TEST_OBJS) -o ecdsa-keygen-test$(EXEEXT)

ecdh-test$(EXEEXT): ecdh-test.$(OBJEXT)
	$(LINK) ecdh-test.$(OBJEXT) $(TEST_OBJS) -o ecdh-test$(EXEEXT)

eddsa-compress-test$(EXEEXT): eddsa-compress-test.$(OBJEXT)
	$(LINK) eddsa-compress-test.$(OBJEXT) $(TEST_OBJS) -o eddsa-compress-test$(EXEEXT)

eddsa-sign-test$(EXEEXT): eddsa-sign-test.$(OBJEXT)
	$(LINK) eddsa-sign-test.$(OBJEXT) $(TEST_OBJS) -o eddsa-sign-test$(EXEEXT)

eddsa-verify-test$(EXEEXT): eddsa-verify-test.$(OBJEXT)
	$(LINK) eddsa-verify-test.$(OBJEXT) $(TEST_OBJS) -o eddsa-verify-test$(EXEEXT)

ed25519-test$(EXEEXT): ed25519-test.$(OBJEXT)
	$(LINK) ed25519-test.$(OBJEXT) $(TEST_OBJS) -o ed25519-test$(EXEEXT)

sha1-huge-test$(EXEEXT): sha1-huge-test.$(OBJEXT)
	$(LINK) sha1-huge-test.$(OBJEXT) $(TEST_OBJS) -o sha1-huge-test$(EXEEXT)

cxx-test$(EXEEXT): cxx-test.$(OBJEXT)
	$(LINK_CXX) cxx-test.$(OBJEXT) $(TEST_OBJS) -o cxx-test$(EXEEXT)

