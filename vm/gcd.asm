        jump.i  #lab0                   ;jump.i  lab0
gcd:
        enter.i #12                     ;enter.i 12
        je.i    *BP+12,#0,#lab1         ;je.i    b,0,lab1
        mov.i   #0,BP-4                 ;mov.i   0,$t0
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        mov.i   #1,BP-4                 ;mov.i   1,$t0
lab2:
        je.i    BP-4,#0,#lab3           ;je.i    $t0,0,lab3
        mov.i   *BP+16,*BP+8            ;mov.i   a,gcd
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mod.i   *BP+16,*BP+12,BP-8      ;mod.i   a,b,$t1
        push.i  BP+12                   ;push.i  &b
        push.i  #BP-8                   ;push.i  &$t1
        push.i  #BP-12                  ;push.i  &$t2
        call.i  #gcd                    ;call.i  &gcd
        incsp.i #12                     ;incsp.i 12
        mov.i   BP-12,*BP+8             ;mov.i   $t2,gcd
lab4:
        leave                           ;leave   
        return                          ;return  
lab0:
        read.i  0                       ;read.i  x
        read.i  4                       ;read.i  y
        push.i  #0                      ;push.i  &x
        push.i  #4                      ;push.i  &y
        push.i  #24                     ;push.i  &$t3
        call.i  #gcd                    ;call.i  &gcd
        incsp.i #12                     ;incsp.i 12
        write.i 24                      ;write.i $t3
        exit                            ;exit    
