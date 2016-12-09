        jump.i  #lab0                   ;jump.i  lab0
f:
        enter.i #28                     ;enter.i 28
        mov.i   #4,BP-4                 ;mov.i   4,q
        add.r   *BP+16,*BP+12,BP-12     ;add.r   a,b,$t0
        inttoreal.i BP-4,BP-20          ;inttoreal.i q,$t1
        add.r   BP-12,BP-20,BP-28       ;add.r   $t0,$t1,$t2
        mov.r   BP-28,*BP+8             ;mov.r   $t2,f
        leave                           ;leave   
        return                          ;return  
lab0:
        mov.r   #3.25,8                 ;mov.r   3.25,g
        push.i  #8                      ;push.i  &g
        mov.r   #10.28,24               ;mov.r   10.28,$t3
        push.i  #24                     ;push.i  &$t3
        push.i  #32                     ;push.i  &$t4
        call.i  #f                      ;call.i  &f
        incsp.i #12                     ;incsp.i 12
        write.r 32                      ;write.r $t4
        exit                            ;exit    
