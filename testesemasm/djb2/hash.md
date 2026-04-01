The djb2 hash algorithm is:

```c
unsigned long hash(unsigned char *src) {
    unsigned long out = 5381;
    for (int i = 0; src[i]; i++) {
        out = ((out << 5) + out) + src[i];

    }

    return out;

}

```

In logical we can do:

```python
#pseudo code
src = "howo"            # Actually howo = [0x68, 0x6f, 0x77, 0x6f, 0x00]
hash = 5381

hashc = hash
hash  = hash << 5
hash += hashc
hash += 104
hash += 111
hash += 119
hash += 111
hash += 0
```
