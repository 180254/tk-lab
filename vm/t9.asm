        jump.i  #lab0                   ;jump.i  lab0
czytajtab:
        enter.i #8                      ;enter.i 8
        sub.i   #1,#1,BP-4              ;sub.i   1,1,$t0
        mul.i   BP-4,#4,BP-4            ;mul.i   $t0,4,$t0
        add.i   BP+8,BP-4,BP-8          ;add.i   &a,$t0,$t1
        mov.i   #10,*BP-8               ;mov.i   10,$t1
        leave                           ;leave   
        return                          ;return  
bubblesort:
        enter.i #0                      ;enter.i 0
        push.i  BP+8                    ;push.i  &a
        call.i  #czytajtab              ;call.i  &czytajtab
        incsp.i #4                      ;incsp.i 4
        leave                           ;leave   
        return                          ;return  
lab0:
        push.i  #12                     ;push.i  &p
        call.i  #czytajtab              ;call.i  &czytajtab
        incsp.i #4                      ;incsp.i 4
        sub.i   #3,#1,56                ;sub.i   3,1,$t2
        mul.i   56,#4,56                ;mul.i   $t2,4,$t2
        add.i   #12,56,60               ;add.i   &p,$t2,$t3
        mov.i   #10,*60                 ;mov.i   10,$t3
        exit                            ;exit    
