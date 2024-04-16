PROGRAM test1;
VAR var1:INTEGER var2:FLOAT;
BEGIN 
	
	var1 := 13;
	var2 := 1;

	IF var1 = 111 
	THEN var1 := 15
	ELSE var1 := 111
	ENDIF; 
	(* illgeal characters *) # % $ & ^ 123 )

	(* unclosed comment
END. 