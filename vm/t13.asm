        jump.i  #lab0                   ;jump.i  lab0
lab0:
lab2:
        jl.i    0,4,#lab3               ;jl.i    i,j,lab3
        mov.i   #0,36                   ;mov.i   0,$t0
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mov.i   #1,36                   ;mov.i   1,$t0
lab4:
        je.i    36,#0,#lab1             ;je.i    $t0,0,lab1
        add.r   12,#1.35,40             ;add.r   x,1.35,$t1
        mov.r   40,12                   ;mov.r   $t1,x
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        exit                            ;exit    
