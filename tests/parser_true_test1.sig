PROGRAM tt1_full_grammar_example;
VAR var1:INTEGER; var2:FLOAT;
BEGIN 

	var1 := 13; 

	IF var1 = 111 
	THEN var1 := var2;
	ELSE 
		IF var2 = var1
		THEN
		ELSE
		ENDIF;
	ENDIF; 

	IF 222 = 222
	THEN var2 := 333;
	ENDIF; 

END. 