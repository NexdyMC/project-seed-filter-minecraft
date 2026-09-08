
## Create Seed Random



```
gcc -shared -o build/seed_random.dll src/seed.c -Wl,--out-implib,build/libseed_random.a -lm
```

```
gcc src/main.c -o build/app_utama.exe -Lbuild -lseed_random
```