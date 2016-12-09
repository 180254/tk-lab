        jump.i  #lab0                   ;jump.i  lab0
lab0:
        jl.i    0,4,#lab1               ;jl.i    i,j,lab1
        mov.i   #0,36                   ;mov.i   0,$t0
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        mov.i   #1,36                   ;mov.i   1,$t0
lab2:
        je.i    36,#0,#lab3             ;je.i    $t0,0,lab3
        mov.r   #0,12                   ;mov.r   0,x
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mov.r   #1,12                   ;mov.r   1,x
lab4:
        exit                            ;exit    
