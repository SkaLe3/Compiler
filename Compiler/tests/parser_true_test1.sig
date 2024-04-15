PROGRAM test1;
VAR var1:INTEGER; var2:FLOAT;
BEGIN 

	var1 := 13; 
	var2 := 1;

	IF var1 = 111 
	THEN var1 := 15;
	ELSE var1 := var2;
	ENDIF; 

	IF 222 = 222
	THEN var2 := 333;
	ENDIF; 

END. 