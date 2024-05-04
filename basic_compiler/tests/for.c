DECLARE FUNCTION printf (fmt AS STRING, ...) AS INTEGER
DECLARE FUNCTION puts (str AS STRING) AS VOID
FUNCTION main () AS INTEGER
    LET i AS INTEGER
    FOR i = 0 TO i < 7
        printf("Test: %d\n", i)
        IF i == 3 THEN
            BREAK
        ENDIF 
    NEXT i = i + 1
    ENDFOR
    RETURN 0
END FUNCTION