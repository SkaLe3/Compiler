PROGRAM test2;
VAR var1:INTEGER var2:FLOAT;
BEGIN 
	
	var1 := 13;
	var2 := 1;

	IF var1 = 111 
	THEN var1 := 15
	ELSE var1 := 111
	ENDIF; 

	(* comment closed with mistake * )
END. 