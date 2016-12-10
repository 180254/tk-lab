        jump.i  #lab0                   ;jump.i  lab0
p:
        enter.i #0                      ;enter.i 0
        leave                           ;leave   
        return                          ;return  
f:
        enter.i #4                      ;enter.i 4
        add.i   *BP+8,#10,BP-4          ;add.i   f,10,$t0
        mov.i   BP-4,*BP+8              ;mov.i   $t0,f
        leave                           ;leave   
        return                          ;return  
lab0:
        call.i  #p                      ;call.i  &p
        push.i  #24                     ;push.i  &$t1
        call.i  #f                      ;call.i  &f
        incsp.i #4                      ;incsp.i 4
        mov.i   24,0                    ;mov.i   $t1,x
        exit                            ;exit    
