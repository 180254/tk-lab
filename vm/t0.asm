        jump.i  #lab0                   ;jump.i  lab0
lab0:
        add.i   0,4,24                  ;add.i   x,y,$t0
        inttoreal.i 24,28               ;inttoreal.i $t0,$t1
        mov.r   28,8                    ;mov.r   $t1,g
        write.r 8                       ;write.r g
        exit                            ;exit    
