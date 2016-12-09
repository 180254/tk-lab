        jump.i  #lab0                   ;jump.i  lab0
f:
        enter.i #4                      ;enter.i 4
        add.i   *BP+12,#10,BP-4         ;add.i   a,10,$t0
        mov.i   BP-4,*BP+8              ;mov.i   $t0,f
        leave                           ;leave   
        return                          ;return  
lab0:
        mov.i   #3,24                   ;mov.i   3,$t1
        push.i  #24                     ;push.i  &$t1
        push.i  #28                     ;push.i  &$t2
        call.i  #f                      ;call.i  &f
        incsp.i #8                      ;incsp.i 8
        mov.i   28,0                    ;mov.i   $t2,x
        exit                            ;exit    
