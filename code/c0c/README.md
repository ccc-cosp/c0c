# c0c 組譯器

## 中間碼轉組合語言

所有的臨時變數都只使用一次 (Single Assignment)，所以都不用保留 (或在組合語言中宣告變數)，只要放在某個暫存器中介，用掉就好了！

例如以下範例中的 t0 ... t7 通通都可以放在 eax 或 R1 中介掉，所以不用保留。

```
    function   sum        int                  
    param      n          int                  
    local      s          int                  
    local      i          int                  
    =          t0         0                    eax = 0
    =          s          t0                   s = eax
    =          t1         1                    eax = 1
    =          i          t1                   i = eax
(WBEGIN2)
    <=         t4         i          n         eax = (i <= n)
    jnz        WEND3      t4                   jnz WEND3 eax
    +          t5         s          i         eax = s + i
    =          s          t5                   s = eax
    =          t6         1                    eax = 1
    +          t7         i          t6        eax = i + eax
    =          i          t7                   i = eax
    jmp        WBEGIN2                         
(WEND3)
    return     s                               eax = s
```

範例 asum.p0

```
    function   total      int                  
    param      a          int        *         
    param      size       int                  
    local      s          int                  
    local      i          int                  
    =          t0         0                    eax = 0
    =          s          t0                   s = eax
    =          t1         0                    eax = 0
    =          i          t1                   i = eax
(WBEGIN2)
    <          t4         i          size      eax = (i<size)
    jnz        WEND3      t4                   jnz WEND3 eax
    []         t5         a          i         eax = a[i]    // 這裡要小心，不要覆蓋掉 eax, 否則就應用其他暫存器。
    +          t6         s          t5        eax = s + eax
    =          s          t6                   s = eax
    jmp        WBEGIN2                         
(WEND3)
    return     s                               eax = s

```



