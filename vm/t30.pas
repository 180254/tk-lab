program sort(input,output);
var a : real;
var b : integer;
var x :array [1..10] of integer;
var y :array [1..10] of real;
begin
  write(a+a);
  write(a+b);

  write(x+x);
  write(y+y);
  write(x+y);

  write(a+x);
  write(b+y);
  write(b+x);
  write(b+y);

  write(a/a);
  write(b/a);
  write(x/a);
  write(x/b);
  write(y/a);
  write(y/b);

  write(a and a);
  write(b and a);

  write(a and x);
  write(a and y);
  write(b and x);
  write(b and y);

  write(a or x);
  write(a or y);
  write(b or x);
  write(b or y);
  write(x or y);

  write(a div x);
  write(a div y);
  write(b div x);
  write(b div y);
  write(x div y);

  write(a mod x);
  write(a mod y);
  write(b mod x);
  write(b mod y);
  write(x mod y);

  write(- a);
  write(- b);
  write(- x);
  write(- y);

  write(- a+x);
  write(- b+a);
  write(- x+y);
  write(- y+3);

  {
  write(+ a);
  write(+ b);
  write(+ x);
  write(+ y)
  }

  write(not a);
  write(not b);
  write(not x);
  write(not y);
  write(not (not (not (not x))))
end.
