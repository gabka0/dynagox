CREATE STREAM R1(src long, dst long) FROM FILE 'R1.dat' LINE DELIMITED CSV;
CREATE STREAM R2(src long, dst long) FROM FILE 'R2.dat' LINE DELIMITED CSV;
CREATE STREAM R3(src long, dst long) FROM FILE 'R3.dat' LINE DELIMITED CSV;
CREATE STREAM R4(src long, dst long) FROM FILE 'R4.dat' LINE DELIMITED CSV;
CREATE STREAM R5(src long, dst long) FROM FILE 'R5.dat' LINE DELIMITED CSV;
CREATE STREAM R6(src long, dst long) FROM FILE 'R6.dat' LINE DELIMITED CSV;
CREATE STREAM R7(src long, dst long) FROM FILE 'R7.dat' LINE DELIMITED CSV;

SELECT COUNT(*) FROM R1, R2, R3, R4, R5, R6, R7
WHERE R1.dst = R2.src AND R2.dst = R3.src AND R3.dst = R1.src AND R3.src = R4.src
AND R4.dst = R5.src AND R5.dst = R6.src AND R6.dst = R7.src AND R7.dst = R5.src;