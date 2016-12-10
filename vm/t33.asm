        jump.i  #lab0                   ;jump.i  lab0
lab0:
        inttoreal.i 0,120               ;inttoreal.i p,$t0
        add.r   40,120,128              ;add.r   z,$t0,$t1
        realtoint.r 128,136             ;realtoint.r $t1,$t2
        mov.i   136,0                   ;mov.i   $t2,p
        exit                            ;exit    
