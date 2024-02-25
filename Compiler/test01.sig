PROGRAM myprogram123;
VAR var1:INTEGER var2:FLOAT;
BEGIN 
	
	var1 := 13;
	var2 := 1;

	IF var1 = 111 
	THEN var1 := 15
	ELSE var1 := 111
	ENDIF; 

	IF 222 = 222
	THEN var2 := 333
	ENDIF; 

	IF 333 = var2
	THEN var2 := 0;
	ENDIF; 
END.