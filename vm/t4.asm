        jump.i  #lab0                   ;jump.i  lab0
f:
        enter.i #4                      ;enter.i 4
        add.i   *BP+12,#10,BP-4         ;add.i   a,10,$t0
        mov.i   BP-4,*BP+8              ;mov.i   $t0,f
        leave                           ;leave   
        return                          ;return  
lab0:
        exit                            ;exit    
