# function   sum                             
# param      n                               
# local      s          var                  
# local      i          var                  
# =          s          0                    
movl 0 eax
movl eax s
# =          i          1                    
movl 1 eax
movl eax i
(WBEGIN0)
# <=         $t0        i          n         
# jnz        WEND1      $t0                  
# +          $t0        s          i         
movl s eax
addl i eax
movl eax $t0
# =          s          $t0                  
movl $t0 eax
movl eax s
# +          $t0        i          1         
movl i eax
addl 1 eax
movl eax $t0
# =          i          $t0                  
movl $t0 eax
movl eax i
# jmp        WBEGIN0                         
(WEND1)
# return     s                               
# fend       sum                             
# push       10                              
# call       $t0        sum                  
# =          s          $t0                  
movl $t0 eax
movl eax s
# str        S2         "sum=%s\n"           
# push       S2                              
# push       s                               
# call       $t0        printf               
