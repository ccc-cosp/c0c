# /          $t0        8          4         
movl 8 eax
divl 4 eax
movl eax $t0
# *          $t1        3          $t0       
movl 3 eax
mull $t0 eax
movl eax $t1
# *          $t2        4          x         
movl 4 eax
mull x eax
movl eax $t2
# +          $t3        $t1        $t2       
movl $t1 eax
addl $t2 eax
movl eax $t3
# -          $t4        $t3        2         
movl $t3 eax
subl 2 eax
movl eax $t4
# =          a          $t4                  
movl $t4 eax
movl eax a
# *          $t0        a          3         
movl a eax
mull 3 eax
movl eax $t0
# +          $t1        $t0        4         
movl $t0 eax
addl 4 eax
movl eax $t1
# -          $t2        $t1        x         
movl $t1 eax
subl x eax
movl eax $t2
# *          $t3        $t2        5         
movl $t2 eax
mull 5 eax
movl eax $t3
# =          b          $t3                  
movl $t3 eax
movl eax b
