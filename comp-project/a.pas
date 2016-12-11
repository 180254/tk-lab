program example(input, output);
var x, y: integer;
var g,h:real;
var oo :array [1..10] of real;

function gcd(a, b: integer):integer;
begin
  if b=0 then 
  begin
    x[oo] := 1;
    gcd:=a
    end
  else 
    gcd:=gcd(b, a mod b)
end;


begin
  read(x, y);
  write(gcd(x, y))
end.
